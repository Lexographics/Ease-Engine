#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#pragma once

#include "visual/viewport.hpp"
#include "visual/window.hpp"

#include "filesystem/filesystem.hpp"

#include "data/project_settings.hpp"

#include "scene/node_db.hpp"
#include "scene/scene.hpp"

class Application {
  public:
	Application(int argc, char const *argv[]);
	~Application();

	void Init();
	void Update();

	inline FileSystem *FS() { return _fs; }

	std::shared_ptr<Scene> NewScene();

  private:
	FileSystem *_fs = nullptr;

	ProjectSettings _projectSettings;

	NodeDB _nodeDB;
	std::shared_ptr<Scene> _currentScene;

	Window _window;
	Viewport _mainViewport;
};

Application &App();

#endif // APPLICATION_HPP