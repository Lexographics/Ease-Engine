#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#pragma once

#include "sowa.hpp"

#include "visual/viewport.hpp"
#include "visual/window.hpp"

#include "filesystem/filesystem.hpp"

#include "data/project_settings.hpp"

#include "scene/node_db.hpp"
#include "scene/scene.hpp"

#include "resource/font.hpp"
#include "resource/resource_registry.hpp"

class Application {
  public:
	Application(int argc, char const *argv[]);
	~Application();

	void Init();
	void Update();

	inline FileSystem *FS() { return _fs; }
	inline NodeDB &GetNodeDB() { return _nodeDB; }
	inline ResourceRegistry &GetResourceRegistry() { return _resourceRegistry; }
	inline Font *GetDefaultFont() { return &_defaultFont; }

	Ref<Scene> NewScene();

  private:
	FileSystem *_fs = nullptr;

	ProjectSettings _projectSettings;

	NodeDB _nodeDB;
	Ref<Scene> _currentScene;

	ResourceRegistry _resourceRegistry;
	Font _defaultFont;

	Window _window;
	Viewport _mainViewport;
};

Application &App();

#endif // APPLICATION_HPP