#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#pragma once

#include <chrono>
#include <list>

#include "eventpp/callbacklist.h"

#include "sowa.hpp"

#include "visual/renderer.hpp"
#include "visual/viewport.hpp"
#include "visual/window.hpp"

#include "filesystem/filesystem.hpp"

#include "core/timer.hpp"
#include "data/project_settings.hpp"
#include "utils/store.hpp"

#include "scene/node_db.hpp"
#include "scene/scene.hpp"

#include "servers/audio_server.hpp"
#include "servers/script_server.hpp"

#include "resource/font.hpp"
#include "resource/resource_registry.hpp"

#include "editor/editor.hpp"

class Application {
  public:
	Application(int argc, char const *argv[]);
	~Application();

	void Init();
	void Update();

	void Start();
	void Stop();

	inline bool IsRunning() { return _isRunning; }

	inline FileSystem &FS() { return _fs; }
	inline NodeDB &GetNodeDB() { return _nodeDB; }
	inline ResourceRegistry &GetResourceRegistry() { return _resourceRegistry; }
	inline Font *GetDefaultFont() { return &_defaultFont; }
	inline Renderer &GetRenderer() { return _renderer; }
	inline Window &GetWindow() { return _window; }
	inline Viewport &GetMainViewport() { return _mainViewport; }
	inline Editor &GetEditor() { return _editor; }
	inline ScriptServer &GetScriptServer() { return _scriptServer; }
	inline AudioServer &GetAudioServer() { return _audioServer; }
	inline ProjectSettings &GetProjectSettings() { return _projectSettings; }

	Ref<Scene> NewScene();
	Ref<Scene> GetCurrentScene();
	void SetCurrentScene(Ref<Scene> scene);

	// Loads scene just before the next frame starts
	void LoadScene(const std::string &path);

	float Delta() { return _delta; }
	inline StringStore &GetGlobalStore() { return _globalStore; }

	inline std::list<Timer> &GetTimers() { return _timers; }
	inline Timer *NewTimer(float timeout, bool autoStart = false) { return &_timers.emplace_back(timeout, autoStart); }

	NodeID GetHoveredID() const { return _hoveredID; }
	void SetCursor(const std::string &texturePath);

  public:
	inline void OnSceneChanged(const std::function<void()> fn) {
		_onSceneChanged.append(fn);
	}

	void RegisterBuiltinData();

  private:
	eventpp::CallbackList<void()> _onSceneChanged;

  private:
	bool _isRunning = false;

	FileSystem _fs;

	ProjectSettings _projectSettings;

	NodeDB _nodeDB;
	Ref<Scene> _currentScene;
	bool _sceneWillLoad = false;
	std::string _sceneToLoad = "";

	Ref<Scene> _backgroundScene;
	ScriptServer _scriptServer;
	AudioServer _audioServer;

	Renderer _renderer;
	ResourceRegistry _resourceRegistry;
	Font _defaultFont;

	Window _window;
	Viewport _mainViewport;

	Editor _editor;
	std::chrono::high_resolution_clock::time_point _lastUpdate;
	float _delta = 0.f;

	StringStore _globalStore;
	StringStore _copyGlobalStore;

	std::list<Timer> _timers;

	NodeID _hoveredID = 0;
};

Application &App();

#endif // APPLICATION_HPP