#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#pragma once

#include "visual/viewport.hpp"
#include "visual/window.hpp"

#include "filesystem/filesystem.hpp"

class Application {
  public:
	Application(int argc, char const *argv[]);
	~Application();

	void Init();
	void Update();

	inline FileSystem *FS() { return _fs; }

  private:
	FileSystem *_fs = nullptr;

	Window _window;
	Viewport _mainViewport;
};

Application &App();

#endif // APPLICATION_HPP