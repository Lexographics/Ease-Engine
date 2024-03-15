#include "input.hpp"
#include "visual/window.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

enum class InputActionState {
	Up = 0,
	Down,
	JustPressed,
	JustReleased,
};

struct InputState {
	Window *window;
	inline GLFWwindow *WindowHandle() { return window->_window; }

	std::unordered_map<std::string, std::vector<Key>> actions;
	std::unordered_map<int, InputActionState> keyStates;
};
static InputState state;

void Input::InitState(Window *window) {
	state.window = window;
}

void Input::Poll() {
	for (auto &[key, keyState] : state.keyStates) {
		if (keyState == InputActionState::JustPressed) {
			keyState = InputActionState::Down;
		} else if (keyState == InputActionState::JustReleased) {
			keyState = InputActionState::Up;
		}
	}

	state.window->PollEvents();
}

bool Input::IsKeyDown(Key key) {
	return state.keyStates[(int)key] == InputActionState::Down || state.keyStates[(int)key] == InputActionState::JustPressed;
}

bool Input::IsKeyJustPressed(Key key) {
	return state.keyStates[(int)key] == InputActionState::JustPressed;
}
bool Input::IsKeyJustReleased(Key key) {
	return state.keyStates[(int)key] == InputActionState::JustReleased;
}

void Input::SetActionKeys(const char *actionName, const std::vector<Key> &keys) {
	state.actions[actionName] = keys;
}

const std::vector<Key> &Input::GetActionKeys(const char *actionName) {
	return state.actions[actionName];
}

bool Input::IsActionPressed(const char *actionName) {
	for (auto key : state.actions[actionName]) {
		if (Input::IsKeyDown(key))
			return true;
	}
	return false;
}

bool Input::IsActionJustPressed(const char *actionName) {
	for (auto key : state.actions[actionName]) {
		if (Input::IsKeyJustPressed(key))
			return true;
	}
	return false;
}

bool Input::IsActionJustReleased(const char *actionName) {
	for (auto key : state.actions[actionName]) {
		if (Input::IsKeyJustReleased(key))
			return true;
	}
	return false;
}

void Input::Callback::Key(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		state.keyStates[key] = InputActionState::JustPressed;
	}

	if (action == GLFW_RELEASE) {
		state.keyStates[key] = InputActionState::JustReleased;
	}
}

void Input::Callback::MouseButton(GLFWwindow *window, int button, int action, int mods) {
	int key = ((int)Input::Key::Mouse1) + button;
	if (action == GLFW_PRESS) {
		state.keyStates[key] = InputActionState::JustPressed;
	}

	if (action == GLFW_RELEASE) {
		state.keyStates[key] = InputActionState::JustReleased;
	}
}