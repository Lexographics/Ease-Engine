#include "script_server.hpp"

#include <LuaBridge/LuaBridge.h>
#include <fmt/args.h>

#include "core/debug.hpp"
#include "math/math.hpp"

#include "scene/node.hpp"
#include "scene/node/animatedsprite2d.hpp"
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
	if (!root) {
		return luabridge::LuaRef(L, nullptr);
	}

	auto ref = luabridge::LuaRef(L, root);
	ref.push(L);
	AssignNodeMetatable(L, root);

	return ref;
}

static luabridge::LuaRef Lua_GetNode(Node *node, std::string path, bool recursive, lua_State *L) {
	Node *n = node->GetNode(path, recursive);
	if (!n) {
		return luabridge::LuaRef(L, nullptr);
	}

	auto ref = luabridge::LuaRef(L, n);
	ref.push(L);
	AssignNodeMetatable(L, n);

	return ref;
}

static luabridge::LuaRef Lua_GetNode(Node *node, std::string path, lua_State *L) {
	return Lua_GetNode(node, path, true, L);
}

static luabridge::LuaRef Lua_Duplicate(Node *node, lua_State *L) {
	Node *dup = node->Duplicate(nullptr);

	auto ref = luabridge::LuaRef(L, dup);
	ref.push(L);
	AssignNodeMetatable(L, dup);

	return ref;
}

static luabridge::LuaRef Lua_GetChild(Node *node, size_t index, lua_State *L) {
	Node *child = node->GetChild(index);
	if (!child) {
		return luabridge::LuaRef(L, nullptr);
	}

	auto ref = luabridge::LuaRef(L, child);
	ref.push(L);
	AssignNodeMetatable(L, child);

	return ref;
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

int ModuleLoader(lua_State *L) {
	const char *path = luaL_checkstring(L, 1);
	return App().GetScriptServer().PushModule(path);
}

int ScriptServer::PushModule(const char *path) {
	Ref<FileData> file = App().FS().Load(path);
	if (!file) {
		Debug::Error("Failed to find module: {}", path);
		lua_pushboolean(state, false);
		return 1;
	}

	std::string mod{reinterpret_cast<char *>(file->Buffer().data()), file->Buffer().size()};

	if (luaL_loadbuffer(state, mod.c_str(), mod.size(), path)) {
		Debug::Error("Failed to run module: {}", path);
		lua_pop(state, 1);

		lua_pushboolean(state, false);
		return 1;
	}

	return 1;
}

void ScriptServer::Init() {
	using namespace luabridge;

	state = luaL_newstate();
	_startFuncs.clear();
	_updateFuncs.clear();

	luaL_openlibs(state);

	lua_register(state, "module_loader", ModuleLoader);
	luaL_dostring(state, "package.searchers = { module_loader }");

	getGlobalNamespace(state)
		.beginNamespace("Debug")
		.addFunction("Log", Lua_DebugLog)
		.addFunction("Info", Lua_DebugInfo)
		.addFunction("Warn", Lua_DebugWarn)
		.addFunction("Error", Lua_DebugError)
		.endNamespace();

	getGlobalNamespace(state)
		.beginNamespace("Key")
		.addVariable("Unknown", Key::Unknown)
		.addVariable("W", Key::W)
		.addVariable("A", Key::A)
		.addVariable("S", Key::S)
		.addVariable("D", Key::D)
		.addVariable("Space", Key::Space)
		.addVariable("Enter", Key::Enter)
		.endNamespace()

		.beginNamespace("Math")
		.addFunction("Clamp", Math::Clamp<float>)
		.endNamespace();

	getGlobalNamespace(state)
		.beginNamespace("Input")
		.addFunction("IsActionPressed", Input::IsActionPressed)
		.addFunction("IsActionJustPressed", Input::IsActionJustPressed)
		.addFunction("IsActionJustReleased", Input::IsActionJustReleased)
		.addFunction("IsKeyDown", +[](i32 key) { return Input::IsKeyDown((Input::Key)key); })
		.addFunction("IsKeyJustPressed", +[](i32 key) { return Input::IsKeyJustPressed((Input::Key)key); })
		.addFunction("IsKeyJustReleased", +[](i32 key) { return Input::IsKeyJustReleased((Input::Key)key); })
		.addFunction("GetActionWeight", Input::GetActionWeight)
		.addFunction("GetActionWeight2", Input::GetActionWeight2)
		.endNamespace();

	getGlobalNamespace(state)
		.addFunction("GetScene", GetScene)

		.beginClass<Vector2>("Vector2")
		.addConstructor<void(), void(float), void(float, float)>()
		.addProperty("x", &Vector2::x)
		.addProperty("y", &Vector2::y)
		.addFunction("Length", &Vector2::Length)
		.addFunction("LengthSquared", &Vector2::LengthSquared)
		.addFunction("DistanceTo", &Vector2::DistanceTo)
		.addFunction("Normalize", &Vector2::Normalize)
		.addFunction("Normalized", &Vector2::Normalized)
		.addFunction("__add", &Vector2::operator+)
		.addFunction("__sub", static_cast<Vector2 (Vector2::*)(const Vector2 &) const>(&Vector2::operator-))
		.addFunction("__mul", static_cast<Vector2 (Vector2::*)(const Vector2 &) const>(&Vector2::operator*), static_cast<Vector2 (Vector2::*)(float) const>(&Vector2::operator*))
		.addFunction("__div", static_cast<Vector2 (Vector2::*)(const Vector2 &) const>(&Vector2::operator/), static_cast<Vector2 (Vector2::*)(float) const>(&Vector2::operator/))
		.addFunction("__unm", static_cast<Vector2 (Vector2::*)() const>(&Vector2::operator-))
		.addFunction("__eq", &Vector2::operator==)
		.addFunction("__tostring", +[](Vector2 &v) { return fmt::format("Vector2({}, {})", v.x, v.y); })
		.endClass()

		.beginClass<Rect>("Rect")
		.addConstructor<void(), void(float, float), void(float, float, float, float)>()
		.addProperty("x", &Rect::x)
		.addProperty("y", &Rect::y)
		.addProperty("w", &Rect::w)
		.addProperty("h", &Rect::h)
		.addFunction("Left", &Rect::Left)
		.addFunction("Right", &Rect::Right)
		.addFunction("Bottom", &Rect::Bottom)
		.addFunction("Top", &Rect::Top)
		.endClass()

		.beginClass<Node>("Node")
		.addFunction("GetNode", luabridge::overload<Node *, std::string, bool, lua_State *>(&Lua_GetNode), luabridge::overload<Node *, std::string, lua_State *>(&Lua_GetNode))
		.addFunction("AddChild", &Node::AddChild)
		.addFunction("GetChildCount", &Node::GetChildCount)
		.addFunction("GetChild", &Lua_GetChild)
		.addFunction("Free", &Node::Free)
		.addFunction("Duplicate", &Lua_Duplicate)
		.addProperty("name", &Node::GetName, &Node::Rename)
		.endClass()

		.deriveClass<Node2D, Node>("Node2D")
		.addProperty("position", &Node2D::_position)
		.addProperty("rotation", &Node2D::_rotation)
		.addProperty("scale", &Node2D::_scale)
		.addProperty("z_index", &Node2D::_zIndex)
		.addProperty("visible", &Node2D::_visible)
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

		.deriveClass<AnimatedSprite2D, Node2D>("AnimatedSprite2D")
		.addProperty("animation_scale", &AnimatedSprite2D::_animationScale)
		.addProperty("playing", &AnimatedSprite2D::_playing)
		.addFunction("SetCurrentAnimation", &AnimatedSprite2D::SetCurrentAnimation, +[](AnimatedSprite2D *node, const std::string &name) { node->SetCurrentAnimation(name, true); })
		.addFunction("GetCurrentAnimation", &AnimatedSprite2D::GetCurrentAnimation)
		.addFunction("RestartAnimation", &AnimatedSprite2D::RestartAnimation)
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
	auto file = App().FS().Load(path);
	if (!file) {
		Debug::Error("Failed to load script file at '{}'", path);
		return;
	}

	std::string str{reinterpret_cast<char *>(file->Data()), file->Size()};
	if (luaL_dostring(state, str.c_str()) != LUA_OK) {
		Debug::Error("Lua load error: {}", lua_tostring(state, -1));
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