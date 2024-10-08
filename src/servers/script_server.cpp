#include "script_server.hpp"

#include <LuaBridge/LuaBridge.h>
#include <fmt/args.h>

#include "core/application.hpp"
#include "core/debug.hpp"
#include "core/timer.hpp"
#include "math/math.hpp"
#include "utils/store.hpp"

#include "scene/node.hpp"
#include "scene/node/animatedsprite2d.hpp"
#include "scene/node/audiostreamplayer.hpp"
#include "scene/node/camera2d.hpp"
#include "scene/node/node2d.hpp"
#include "scene/node/progress_bar.hpp"
#include "scene/node/sprite2d.hpp"
#include "scene/node/text2d.hpp"
#include "scene/scene.hpp"

static void PrintTable(luabridge::LuaRef ref);

struct NodeScriptData {
	int tableRef;
	Node *node;
};

struct ScriptServerData {
	void MakeNodeScriptTemplate(const std::string &path, int registryRef) {
		_nodeScriptTemplates[path] = registryRef;
	}
	int GetNodeScriptTemplate(const std::string &name) {
		if (_nodeScriptTemplates.find(name) == _nodeScriptTemplates.end()) {
			return -1;
		}

		return _nodeScriptTemplates[name];
	}
	NodeScriptID MakeNodeScript(NodeScriptData data) {
		static LinearIDGenerator<NodeScriptID> gen;
		NodeScriptID id = gen.Next();

		_nodeScripts[id] = data;
		return id;
	}
	void DestroyNodeScript(NodeScriptID id, lua_State *L) {
		if (_nodeScripts.find(id) == _nodeScripts.end())
			return;

		luaL_unref(L, LUA_REGISTRYINDEX, _nodeScripts[id].tableRef);

		_nodeScripts.erase(_nodeScripts.find(id));
	}
	const std::unordered_map<NodeScriptID, NodeScriptData> &GetNodeScripts() {
		return _nodeScripts;
	}

  private:
	// path -> LUA_REGISTRYINDEX
	std::unordered_map<std::string, int> _nodeScriptTemplates;
	std::unordered_map<NodeScriptID, NodeScriptData> _nodeScripts;
};

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

ScriptServer::ScriptServer() {
}

ScriptServer::~ScriptServer() {
}

void ScriptServer::Init() {
	using namespace luabridge;

	state = luaL_newstate();
	data = std::make_unique<ScriptServerData>();

	luaopen_base(state);

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
		.addFunction("NodeScript", +[](const std::string &name, const std::string &extends, lua_State *L) -> luabridge::LuaRef {
			auto script = luabridge::LuaRef::newTable(L);
			script["__extends"] = extends;
			script["__script_name"] = name;

			return script; })

		.beginClass<Tween>("Tween")
		.endClass()

		.beginClass<Timer>("Timer")
		.addFunction("Start", &Timer::Start)
		.addFunction("Pause", &Timer::Pause)
		.addFunction("Stop", &Timer::Stop)
		.addFunction("OnTimeout", +[](Timer *timer, luabridge::LuaRef func, lua_State *L) {
																								if (!func.isCallable()) {
																									lua_pushstring(L, "OnTimeout argument must be a function");
																									lua_error(L);
																									return;
																								};
																								
																							 timer->OnTimeout([func]() {
																								func.call();
																								}); })
		.endClass()
		.addFunction("NewTimer", +[](float timeout) { return App().NewTimer(timeout); }, +[](float timeout, bool autoStart) { return App().NewTimer(timeout, autoStart); })
		.addFunction("NewTween", +[](float duration, int easing, luabridge::LuaRef callback, luabridge::LuaRef onFinish) {
				Tween* tween = App().NewTween(duration, (Easing)easing, nullptr, nullptr);
				if(callback.isCallable()) {
					tween->SetCallback([callback](float value){callback.call(value);});
				}
				if(onFinish.isCallable()) {
					tween->SetOnFinish([onFinish](){ onFinish.call(); });
				}
				return tween;
				; })
		.addFunction("LoadScene", +[](const std::string &path) { App().LoadScene(path); })

		.beginNamespace("Time")
		.addProperty("delta", +[]() { return App().Delta(); })
		.endNamespace()

		.beginNamespace("Key")
		.addVariable("Unknown", Key::Unknown)
		.addVariable("W", Key::W)
		.addVariable("A", Key::A)
		.addVariable("S", Key::S)
		.addVariable("D", Key::D)
		.addVariable("E", Key::E)
		.addVariable("Space", Key::Space)
		.addVariable("Enter", Key::Enter)
		.addVariable("Escape", Key::Escape)
		.addVariable("MouseLeft", Key::MouseLeft)
		.addVariable("MouseMiddle", Key::MouseMiddle)
		.addVariable("MouseRight", Key::MouseRight)
		.endNamespace()

		.beginNamespace("Easing")
		.addVariable("Linear", Easing::Linear)
		.addVariable("SineEaseIn", Easing::SineEaseIn)
		.addVariable("SineEaseOut", Easing::SineEaseOut)
		.addVariable("SineEaseInOut", Easing::SineEaseInOut)
		.addVariable("CubicEaseIn", Easing::CubicEaseIn)
		.addVariable("CubicEaseOut", Easing::CubicEaseOut)
		.addVariable("CubicEaseInOut", Easing::CubicEaseInOut)
		.addVariable("QuintIn", Easing::QuintIn)
		.addVariable("QuintOut", Easing::QuintOut)
		.addVariable("QuintInOut", Easing::QuintInOut)
		.addVariable("CircIn", Easing::CircIn)
		.addVariable("CircOut", Easing::CircOut)
		.addVariable("CircInOut", Easing::CircInOut)
		.addVariable("ElasticIn", Easing::ElasticIn)
		.addVariable("ElasticOut", Easing::ElasticOut)
		.addVariable("ElasticInOut", Easing::ElasticInOut)
		.addVariable("QuadIn", Easing::QuadIn)
		.addVariable("QuadOut", Easing::QuadOut)
		.addVariable("QuadInOut", Easing::QuadInOut)
		.addVariable("QuartIn", Easing::QuartIn)
		.addVariable("QuartOut", Easing::QuartOut)
		.addVariable("QuartInOut", Easing::QuartInOut)
		.addVariable("ExpoIn", Easing::ExpoIn)
		.addVariable("ExpoOut", Easing::ExpoOut)
		.addVariable("ExpoInOut", Easing::ExpoInOut)
		.addVariable("BackIn", Easing::BackIn)
		.addVariable("BackOut", Easing::BackOut)
		.addVariable("BackInOut", Easing::BackInOut)
		.addVariable("BounceIn", Easing::BounceIn)
		.addVariable("BounceOut", Easing::BounceOut)
		.addVariable("BounceInOut", Easing::BounceInOut)
		.endNamespace()

		.beginNamespace("Math")
		.addFunction("Clamp", Math::Clamp<float>)
		.addFunction("Lerp", Math::Lerp<float>, Math::Lerp<Vector2>)
		.addFunction("Atan2", Math::Atan2)
		.addFunction("Map", Math::Map<float>)
		.addFunction("TweenMap", Math::TweenMap<float>)
		.endNamespace()

		.beginNamespace("Utils")
		.addFunction("Rand", Utils::Rand)
		.addFunction("RandFloat", Utils::RandFloat)
		.addFunction("Randomize", Utils::Randomize)
		.addFunction("RandRange", Utils::RandRange)
		.addFunction("RandRangeFloat", Utils::RandRangeFloat)
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
		.addFunction("GetWindowMousePosition", Input::GetWindowMousePosition)
		.addFunction("GetMousePosition", Input::GetMousePosition)
		.endNamespace();

	getGlobalNamespace(state)
		.addFunction("GetScene", +[]() { return App().GetCurrentScene().get(); })
		.addFunction("GlobalStore", +[]() { return &App().GetGlobalStore(); })

		.beginClass<StringStore>("StringStore")
		.addFunction("Clear", &StringStore::Clear)
		.addFunction("Set", &StringStore::Set)
		.addFunction("Get", &StringStore::Get)
		.addFunction("Has", &StringStore::Has)
		.addFunction("Remove", &StringStore::Remove)
		.endClass()

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

		.beginClass<Color>("Color")
		.addConstructor<void(), void(float, float, float), void(float, float, float, float)>()
		.addProperty("r", &Color::r)
		.addProperty("g", &Color::g)
		.addProperty("b", &Color::b)
		.addProperty("a", &Color::a)
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
		.addFunction("IsInGroup", &Node::IsInGroup)
		.addFunction("AddGroup", &Node::AddGroup)
		.addFunction("RemoveGroup", &Node::RemoveGroup)
		.addFunction("GetID", &Node::GetID)
		.addFunction("IsHovered", &Node::IsHovered)
		.addProperty("name", &Node::GetName, &Node::Rename)
		.endClass()

		.deriveClass<Node2D, Node>("Node2D")
		.addFunction("GetGlobalPosition", &Node2D::GetGlobalPosition)
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
		.addProperty("offset", &Camera2D::_offset)
		.addProperty("rotatable", &Camera2D::_rotatable)
		.endClass()

		.deriveClass<AnimatedSprite2D, Node2D>("AnimatedSprite2D")
		.addProperty("animation_scale", &AnimatedSprite2D::_animationScale)
		.addProperty("playing", &AnimatedSprite2D::_playing)
		.addProperty("modulate", &AnimatedSprite2D::_modulate)
		.addFunction("SetCurrentAnimation", &AnimatedSprite2D::SetCurrentAnimation, +[](AnimatedSprite2D *node, const std::string &name) { node->SetCurrentAnimation(name, true); })
		.addFunction("GetCurrentAnimation", &AnimatedSprite2D::GetCurrentAnimation)
		.addFunction("RestartAnimation", &AnimatedSprite2D::RestartAnimation)
		.endClass()

		.deriveClass<AudioStreamPlayer, Node>("AudioStreamPlayer")
		.addFunction("Play", &AudioStreamPlayer::Play)
		.addFunction("Pause", &AudioStreamPlayer::Pause)
		.addFunction("Stop", &AudioStreamPlayer::Stop)
		.addFunction("IsPlaying", &AudioStreamPlayer::IsPlaying)
		.addFunction("IsPaused", &AudioStreamPlayer::IsPaused)
		.addProperty("stream", &AudioStreamPlayer::_stream)
		.addProperty("autoplay", &AudioStreamPlayer::_autoplay)
		.addProperty("loop", &AudioStreamPlayer::_loop)
		.addProperty("pitch", &AudioStreamPlayer::_pitch)
		.addProperty("gain", &AudioStreamPlayer::_gain)
		.endClass()

		.deriveClass<ProgressBar, Node2D>("ProgressBar")
		.addProperty("min_value", &ProgressBar::_minValue)
		.addProperty("max_value", &ProgressBar::_maxValue)
		.addProperty("value", &ProgressBar::_value)
		.addProperty("size", &ProgressBar::_size)
		.addProperty("padding", &ProgressBar::_padding)
		.addProperty("foreground_color", &ProgressBar::_foregroundColor)
		.addProperty("background_color", &ProgressBar::_backgroundColor)
		.endClass()

		.beginClass<Scene>("Scene")
		.addFunction("GetRoot", Lua_GetRoot)
		.endClass();
}

void ScriptServer::CallStart() {
	for (auto &[id, refIndex] : data->GetNodeScripts()) {
		lua_rawgeti(state, LUA_REGISTRYINDEX, refIndex.tableRef);
		luabridge::LuaRef ref = luabridge::LuaRef::fromStack(state);

		if (ref.isTable()) {
			auto fn = ref["Start"];
			if (fn.isFunction()) {
				fn.push(state);
				ref.push(state);

				auto node = luabridge::LuaRef(state, refIndex.node);
				node.push(state);
				AssignNodeMetatable(state, refIndex.node);

				if (lua_pcall(state, 2, 0, 0) != LUA_OK) {
					Debug::Error("Lua start error: {}", lua_tostring(state, -1));
				}
			}
		}
	}
}

void ScriptServer::CallUpdate() {
	for (auto &[id, refIndex] : data->GetNodeScripts()) {
		lua_rawgeti(state, LUA_REGISTRYINDEX, refIndex.tableRef);
		luabridge::LuaRef ref = luabridge::LuaRef::fromStack(state);

		if (ref.isTable()) {
			auto fn = ref["Update"];
			if (fn.isFunction()) {
				fn.push(state);
				ref.push(state);

				auto node = luabridge::LuaRef(state, refIndex.node);
				node.push(state);
				AssignNodeMetatable(state, refIndex.node);

				if (lua_pcall(state, 2, 0, 0) != LUA_OK) {
					Debug::Error("Lua update error: {}", lua_tostring(state, -1));
				}
			}
		}
	}
}

bool ScriptServer::LoadNodeScript(const char *path) {
	auto file = App().FS().Load(path);
	if (!file) {
		Debug::Error("Failed to load script file at '{}'", path);
		return false;
	}

	std::string str{reinterpret_cast<char *>(file->Data()), file->Size()};
	if (luaL_dostring(state, str.c_str()) != LUA_OK) {
		Debug::Error("Lua load error: {}", lua_tostring(state, -1));
		return false;
	}

	int refIndex = luaL_ref(state, LUA_REGISTRYINDEX);
	lua_rawgeti(state, LUA_REGISTRYINDEX, refIndex);

	luabridge::LuaRef luaRef = luabridge::LuaRef::fromStack(state);
	if (!luaRef.isTable()) {
		Debug::Error("Invalid module file. Expected table");
		return false;
	}

	if (!luaRef["__script_name"].isString()) {
		Debug::Error("Script name not found. Use NodeScript(name, extends) or SceneScript(name)");
		return false;
	}

	std::string name = luaRef["__script_name"].tostring();

	data->MakeNodeScriptTemplate(path, refIndex);

	return true;
}

void ScriptServer::NewNodeScript(Node *node, NodeScriptRef &ref, const std::string &scriptPath) {
	int refIndex = data->GetNodeScriptTemplate(scriptPath);
	if (refIndex < 0) {
		LoadNodeScript(scriptPath.c_str());

		refIndex = data->GetNodeScriptTemplate(scriptPath);
		if (refIndex < 0) {
			Debug::Error("1: Invalid node script : {}", scriptPath);
			return;
		}
	}

	lua_rawgeti(state, LUA_REGISTRYINDEX, refIndex);
	luabridge::LuaRef scriptTemplate = luabridge::LuaRef::fromStack(state);
	if (!scriptTemplate.isValid() || !scriptTemplate.isTable()) {
		Debug::Error("2: Invalid node script : {}", scriptPath);
		return;
	}

	lua_newtable(state);
	luabridge::LuaRef scriptInstance = luabridge::LuaRef::fromStack(state);
	scriptInstance.push(state);

	lua_pushstring(state, "__extends");
	scriptTemplate["__extends"].push(state);
	lua_settable(state, -3);

	lua_pushstring(state, "__script_name");
	scriptTemplate["__script_name"].push(state);
	lua_settable(state, -3);

	lua_pushstring(state, "Start");
	scriptTemplate["Start"].push(state);
	lua_settable(state, -3);

	lua_pushstring(state, "Update");
	scriptTemplate["Update"].push(state);
	lua_settable(state, -3);

	int instanceRef = luaL_ref(state, LUA_REGISTRYINDEX);

	NodeScriptID scriptID = data->MakeNodeScript(NodeScriptData{
		.tableRef = instanceRef,
		.node = node,
	});

	ref._scripts.push_back(NodeScriptInstance{
		.scriptID = scriptID,
		.path = scriptPath,
	});
}

void ScriptServer::DestroyNodeScript(NodeScriptID id) {
	data->DestroyNodeScript(id, state);
}

NodeScriptRef::~NodeScriptRef() {
	Clear();
}

void NodeScriptRef::Clear() {
	for (const NodeScriptInstance &script : _scripts) {
		App().GetScriptServer().DestroyNodeScript(script.scriptID);
	}
	_scripts.clear();
}

void PrintTable(luabridge::LuaRef ref) {
	ref.push();

	lua_pushnil(ref.state()); // First key
	while (lua_next(ref.state(), -2) != 0) {

		if (lua_istable(ref.state(), -1)) {
			PrintTable(luabridge::LuaRef::fromStack(ref.state(), -1));
		} else if (lua_isstring(ref.state(), -1)) {
			Debug::Log("Key={}, Value={}", lua_tostring(ref.state(), -2), lua_tostring(ref.state(), -1));
		} else if (lua_isnumber(ref.state(), -1)) {
			Debug::Log("Key={}, Value={}", lua_tostring(ref.state(), -2), lua_tonumber(ref.state(), -1));
		} else if (lua_isboolean(ref.state(), -1)) {
			Debug::Log("Key={}, Value={}", lua_tostring(ref.state(), -2), lua_toboolean(ref.state(), -1));
		} else if (lua_isfunction(ref.state(), -1)) {
			Debug::Log("Key={}, Value={}", lua_tostring(ref.state(), -2), "function");
		}

		lua_pop(ref.state(), 1);
	}
};