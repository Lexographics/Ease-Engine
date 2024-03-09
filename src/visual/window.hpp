#ifndef WINDOW_HPP
#define WINDOW_HPP
#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

class Window {
  public:
	Window();
	~Window();

	void Create(int width, int height, const char *title);

	bool ShouldClose();
	void SwapBuffers();
	void PollEvents();

  private:
	GLFWwindow *_window;
};

#endif // WINDOW_HPP