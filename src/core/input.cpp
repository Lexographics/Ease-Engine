#include "input.hpp"
#include "visual/window.hpp"

#include <iostream>

struct InputState {
	Window *window;
	inline GLFWwindow *WindowHandle() { return window->_window; }
};
static InputState state;

void Input::InitState(Window *window) {
	state.window = window;
}

bool Input::IsKeyDown(Key key) {
	return glfwGetKey(state.WindowHandle(), (int)key) == GLFW_PRESS;
}

void Input::Callback::Key(GLFWwindow *window, int key, int scancode, int action, int mods) {
}