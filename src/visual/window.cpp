#include "window.hpp"
#include <iostream>
#include <stdexcept>

static void ErrorCallback(int error, const char *description);

Window::Window() {
}

Window::~Window() {
	glfwDestroyWindow(_window);
	glfwTerminate();
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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to load glad");
	}
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

void ErrorCallback(int error, const char *description) {
	fprintf(stderr, "Error: %s\n", description);
}