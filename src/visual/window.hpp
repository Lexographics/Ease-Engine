#ifndef WINDOW_HPP
#define WINDOW_HPP
#pragma once

#include "visual/gl.hpp"

#include <glm/glm.hpp>

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

	glm::vec2 GetWindowSize();

  private:
	friend struct InputState;
	friend struct WindowAccessor;
	GLFWwindow *_window;

	int _width = 0;
	int _height = 0;
};

#endif // WINDOW_HPP