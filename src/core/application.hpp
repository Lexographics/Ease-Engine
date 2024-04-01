#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#pragma once

#include "visual/viewport.hpp"
#include "visual/window.hpp"

class Application {
  public:
	Application(int argc, char const *argv[]);
	~Application();

	void Init();
	void Update();

  private:
	Window _window;

	Viewport _mainViewport;
};

#endif // APPLICATION_HPP