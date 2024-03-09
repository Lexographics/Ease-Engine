#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#pragma once

#include "visual/window.hpp"

class Application {
  public:
	Application(int argc, char const *argv[]);
	~Application();

	void Start();

  private:
	Window _window;
};

#endif // APPLICATION_HPP