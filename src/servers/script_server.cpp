#include "script_server.hpp"

#include <LuaBridge/LuaBridge.h>
#include <fmt/args.h>

#include "core/debug.hpp"

#include "scene/node.hpp"
#include "scene/node/node2d.hpp"
#include "scene/node/sprite2d.hpp"
#include "scene/node/text2d.hpp"
#include "scene/scene.hpp"

static Scene *GetScene() {
	return App().GetCurrentScene().get();
}

static void AssignNodeMetatable(lua_State *L, Node *node) {
	// lua_pushlightuserdata(L, node);
	lua_rawgetp(L, LUA_REGISTRYINDEX, App().GetNodeDB().GetNodeType(node->TypeID()).scriptKey);
	// lua_rawgetp(L, LUA_REGISTRYINDEX, luabridge::detail::getClassRegistryKey<Node>());
	lua_setmetatable(L, -2);
}

static luabridge::LuaRef Lua_GetRoot(Scene *scene, lua_State *L) {
	Node *root = scene->GetRoot();

	auto ref = luabridge::LuaRef(L, root);
	ref.push(L);
	AssignNodeMetatable(L, root);

	return ref;
}

static luabridge::LuaRef Lua_GetNode(Node *node, std::string path, bool recursive, lua_State *L) {
	Node *n = node->GetNode(path, recursive);

	auto ref = luabridge::LuaRef(L, n);
	ref.push(L);
	AssignNodeMetatable(L, n);

	return ref;
}

static luabridge::LuaRef Lua_GetNode(Node *node, std::string path, lua_State *L) {
	return Lua_GetNode(node, path, true, L);
}

static int Lua_DebugPrint(lua_State *L, Debug::LogSeverity severity) {
	int count = lua_gettop(L);

	auto store = fmt::dynamic_format_arg_store<fmt::format_context>();

	const char *format = lua_tostring(L, 1);
	for (int i = 2; i <= count; i++) {
		if (lua_isboolean(L, i)) {
			store.push_back(lua_toboolean(L, i) ? "true" : "false");
		} else {
			size_t len;
			const char *str = luaL_tolstring(L, i, &len);
			if (str) {
				store.push_back(std::string(str));
			} else {
				store.push_back("nil");
			}
		}
	}

	try {
		std::string msg = fmt::vformat(format, store);
		Debug::Internal::Print(msg, Debug::LogSeverityToString(severity), severity);
	} catch (const std::exception &e) {
		Debug::Error("Format error: {}", e.what());
	}
	return 0;
}

static int Lua_DebugLog(lua_State *L) {
	return Lua_DebugPrint(L, Debug::LogSeverity::Log);
}
static int Lua_DebugInfo(lua_State *L) {
	return Lua_DebugPrint(L, Debug::LogSeverity::Info);
}
static int Lua_DebugWarn(lua_State *L) {
	return Lua_DebugPrint(L, Debug::LogSeverity::Warn);
}
static int Lua_DebugError(lua_State *L) {
	return Lua_DebugPrint(L, Debug::LogSeverity::Error);
}

template <>
struct luabridge::Stack<Key> : luabridge::Enum<Key> {};

void ScriptServer::Init() {
	using namespace luabridge;

	state = luaL_newstate();
	_startFuncs.clear();
	_updateFuncs.clear();

	luaL_openlibs(state);

	getGlobalNamespace(state)
		.beginNamespace("Debug")
		.addFunction("Log", Lua_DebugLog)
		.addFunction("Info", Lua_DebugInfo)
		.addFunction("Warn", Lua_DebugWarn)
		.addFunction("Error", Lua_DebugError)
		.endNamespace();

	getGlobalNamespace(state)
		.beginNamespace("Key")
		.addVariable("W", Key::W)
		.addVariable("Space", Key::Space)
		.endNamespace();

	getGlobalNamespace(state)
		.beginNamespace("Input")
		.addFunction("IsActionPressed", Input::IsActionPressed)
		.addFunction("IsActionJustPressed", Input::IsActionJustPressed)
		.addFunction("IsActionJustReleased", Input::IsActionJustReleased)
		.addFunction("IsKeyDown", +[](i32 key) { return Input::IsKeyDown((Input::Key)key); })
		.addFunction("IsKeyJustPressed", +[](i32 key) { return Input::IsKeyJustPressed((Input::Key)key); })
		.addFunction("IsKeyJustReleased", +[](i32 key) { return Input::IsKeyJustReleased((Input::Key)key); })
		.endNamespace();

	getGlobalNamespace(state)
		.addFunction("GetScene", GetScene)

		.beginClass<glm::vec2>("Vector2")
		.addConstructor<void(), void(float), void(float, float)>()
		.addProperty("x", &glm::vec2::x)
		.addProperty("y", &glm::vec2::y)
		.addFunction("__tostring", [](glm::vec2 &v) { return fmt::format("Vector2({}, {})", v.x, v.y); })
		.endClass()

		.beginClass<Node>("Node")
		.addFunction("GetNode",
					 luabridge::overload<Node *, std::string, bool, lua_State *>(&Lua_GetNode),
					 luabridge::overload<Node *, std::string, lua_State *>(&Lua_GetNode))
		.addProperty("name", &Node::GetName, &Node::Rename)
		.endClass()

		.deriveClass<Node2D, Node>("Node2D")
		.addProperty("position", &Node2D::_position)
		.addProperty("rotation", &Node2D::_rotation)
		.addProperty("scale", &Node2D::_scale)
		.endClass()

		.deriveClass<Sprite2D, Node2D>("Sprite2D")
		.addProperty("texture", &Sprite2D::_texture)
		.addProperty("modulate", &Sprite2D::_modulate)
		.endClass()

		.deriveClass<Text2D, Node2D>("Text2D")
		.addProperty("text", &Text2D::_text)
		.addProperty("modulate", &Text2D::_modulate)
		.addProperty("font", &Text2D::_font)
		.endClass()

		.deriveClass<Camera2D, Node2D>("Camera2D")
		.endClass()

		.beginClass<Scene>("Scene")
		.addFunction("GetRoot", Lua_GetRoot)
		.endClass();
}

void ScriptServer::CallStart() {
	for (auto &fun : _startFuncs) {
		lua_rawgeti(state, LUA_REGISTRYINDEX, fun);
		if (lua_pcall(state, 0, 0, 0) != LUA_OK) {
			Debug::Error("Lua start error: {}", lua_tostring(state, -1));
		}
	}
}

void ScriptServer::CallUpdate() {
	for (auto &fun : _updateFuncs) {
		lua_rawgeti(state, LUA_REGISTRYINDEX, fun);
		if (lua_pcall(state, 0, 0, 0) != LUA_OK) {
			Debug::Error("Lua update error: {}", lua_tostring(state, -1));
		}
	}
}

void ScriptServer::LoadScript(const char *path) {
	auto file = App().FS()->Load(path);
	if (!file) {
		Debug::Error("Failed to load script file at '{}'", path);
		return;
	}

	std::string str{reinterpret_cast<char *>(file->buffer.data()), file->buffer.size()};
	if (luaL_dostring(state, str.c_str()) != LUA_OK) {
		Debug::Error("Lua update error: {}", lua_tostring(state, -1));
	}

	lua_getglobal(state, "Start");
	if (lua_isfunction(state, -1)) {
		_startFuncs.push_back(luaL_ref(state, LUA_REGISTRYINDEX));

		lua_pushnil(state);
		lua_setglobal(state, "Start");
	}

	lua_getglobal(state, "Update");
	if (lua_isfunction(state, -1)) {
		_updateFuncs.push_back(luaL_ref(state, LUA_REGISTRYINDEX));

		lua_pushnil(state);
		lua_setglobal(state, "Update");
	}
}