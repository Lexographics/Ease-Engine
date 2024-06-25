#include "window.hpp"
#include <iostream>
#include <stdexcept>

static void ErrorCallback(int error, const char *description);
static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);

static Window *s_window;

struct WindowAccessor {
	static void SetSize(int width, int height) {
		s_window->_width = width;
		s_window->_height = height;
	}
};

Window::Window() {
	s_window = this;
}

Window::~Window() {
	glfwDestroyWindow(_window);
}

void Window::Create(int width, int height, const char *title) {
	glfwInit();
	glfwSetErrorCallback(ErrorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!_window) {
		throw std::runtime_error("Failed to create window");
	}

	glfwMakeContextCurrent(_window);
#ifndef SW_WEB
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to load glad");
	}
#endif

	glfwSetKeyCallback(_window, Input::Callback::Key);
	glfwSetMouseButtonCallback(_window, Input::Callback::MouseButton);
	glfwSetCursorPosCallback(_window, Input::Callback::MouseMove);
	glfwSetScrollCallback(_window, Input::Callback::Scroll);

	glfwSetFramebufferSizeCallback(_window, FramebufferSizeCallback);

	_width = width;
	_height = height;
}

bool Window::ShouldClose() {
	return glfwWindowShouldClose(_window);
}

void Window::SwapBuffers() {
	glfwSwapBuffers(_window);
}

void Window::PollEvents() {
	glfwPollEvents();
}

void Window::SetShouldClose(bool close /*= true*/) {
	glfwSetWindowShouldClose(_window, close ? 1 : 0);
}

glm::vec2 Window::GetWindowSize() {
	return glm::vec2(_width, _height);
}

void FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
	WindowAccessor::SetSize(width, height);
}

void ErrorCallback(int error, const char *description) {
	fprintf(stderr, "Error: %s\n", description);
}