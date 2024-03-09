#ifndef WINDOW_HPP
#define WINDOW_HPP
#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "core/input.hpp"

class Window {
  public:
	Window();
	~Window();

	void Create(int width, int height, const char *title);

	bool ShouldClose();
	void SwapBuffers();
	void PollEvents();

	void SetShouldClose(bool close = true);

  private:
	friend class InputState;
	GLFWwindow *_window;
};

#endif // WINDOW_HPP