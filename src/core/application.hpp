#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#pragma once

#include "eventpp/callbacklist.h"

#include "sowa.hpp"

#include "visual/renderer.hpp"
#include "visual/viewport.hpp"
#include "visual/window.hpp"

#include "filesystem/filesystem.hpp"

#include "data/project_settings.hpp"

#include "scene/node_db.hpp"
#include "scene/scene.hpp"

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

	inline FileSystem *FS() { return _fs; }
	inline NodeDB &GetNodeDB() { return _nodeDB; }
	inline ResourceRegistry &GetResourceRegistry() { return _resourceRegistry; }
	inline Font *GetDefaultFont() { return &_defaultFont; }
	inline Renderer &GetRenderer() { return _renderer; }
	inline Window &GetWindow() { return _window; }
	inline Viewport &GetMainViewport() { return _mainViewport; }
	inline Editor &GetEditor() { return _editor; }

	Ref<Scene> NewScene();
	Ref<Scene> GetCurrentScene();
	void SetCurrentScene(Ref<Scene> scene);

  public:
	inline void OnSceneChanged(const std::function<void()> fn) {
		_onSceneChanged.append(fn);
	}

  private:
	eventpp::CallbackList<void()> _onSceneChanged;

  private:
	bool _isRunning = false;

	FileSystem *_fs = nullptr;

	ProjectSettings _projectSettings;

	NodeDB _nodeDB;
	Ref<Scene> _currentScene;
	Ref<Scene> _backgroundScene;
	ScriptServer _scriptServer;

	Renderer _renderer;
	ResourceRegistry _resourceRegistry;
	Font _defaultFont;

	Window _window;
	Viewport _mainViewport;

	Editor _editor;
};

Application &App();

#endif // APPLICATION_HPP