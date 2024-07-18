#include "input.hpp"
#include "visual/window.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "core/application.hpp"
#include "core/debug.hpp"

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

	eventpp::CallbackList<void(Input::Event e)> inputEvent;

	Vector2 cursorPos = Vector2(0.f, 0.f);
	Vector2 lastMouseScroll = Vector2(0.f, 0.f);
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

	state.lastMouseScroll = Vector2(0.f, 0.f);

	state.window->PollEvents();
}

bool Input::IsKeyDown(Key key) {
	if (App().GetEditor().HasFocus())
		return false;
	return state.keyStates[(int)key] == InputActionState::Down || state.keyStates[(int)key] == InputActionState::JustPressed;
}

bool Input::IsKeyJustPressed(Key key) {
	if (App().GetEditor().HasFocus())
		return false;
	return state.keyStates[(int)key] == InputActionState::JustPressed;
}
bool Input::IsKeyJustReleased(Key key) {
	if (App().GetEditor().HasFocus())
		return false;
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

float Input::GetActionWeight(const char *negativeAction, const char *positiveAction) {
	return -static_cast<int>(IsActionPressed(negativeAction)) + static_cast<int>(IsActionPressed(positiveAction));
}
Vector2 Input::GetActionWeight2(const char *negativeActionX, const char *positiveActionX, const char *negativeActionY, const char *positiveActionY) {
	return Vector2(GetActionWeight(negativeActionX, positiveActionX), GetActionWeight(negativeActionY, positiveActionY));
}

Vector2 Input::GetWindowMousePosition() {
	return state.cursorPos;
}

Vector2 Input::GetMousePosition() {
#ifndef SW_EDITOR
	Vector2 windowSize = state.window->GetWindowSize();
	Vector2 videoSize = Vector2(App().GetProjectSettings().rendering.viewport.width, App().GetProjectSettings().rendering.viewport.height);
	Vector2 pos = GetWindowMousePosition();
	pos.y = windowSize.y - pos.y;
	pos = App().GetViewportRect().MapPoint(pos, Rect(0, 0, videoSize.x, videoSize.y));
	return pos;

#else
	Vector2 videoSize = Vector2(App().GetProjectSettings().rendering.viewport.width, App().GetProjectSettings().rendering.viewport.height);
	Vector2 pos = GetWindowMousePosition();
	pos = App().GetEditor().GetViewportRect().MapPoint(pos, Rect(0, 0, videoSize.x, videoSize.y));
	return pos * Vector2(1, -1);
#endif
}

float Input::GetMouseScrollY() {
	return state.lastMouseScroll.y;
}

float Input::GetMouseScrollX() {
	return state.lastMouseScroll.x;
}

eventpp::CallbackList<void(Input::Event e)> &Input::InputEvent() {
	return state.inputEvent;
}

void Input::Callback::Key(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		state.keyStates[key] = InputActionState::JustPressed;
	}

	if (action == GLFW_RELEASE) {
		state.keyStates[key] = InputActionState::JustReleased;
	}

	Input::Event event;
	event.type = Input::EventType::Key;
	event.key.key = (Input::Key)key;
	event.key.action = action;
	state.inputEvent(event);
}

void Input::Callback::MouseButton(GLFWwindow *window, int button, int action, int mods) {
	int key = ((int)Input::Key::Mouse1) + button;
	if (action == GLFW_PRESS) {
		state.keyStates[key] = InputActionState::JustPressed;
	}

	if (action == GLFW_RELEASE) {
		state.keyStates[key] = InputActionState::JustReleased;
	}

	Input::Event event;
	event.type = Input::EventType::Button;
	event.button.key = (Input::Key)key;
	event.button.action = action;
	state.inputEvent(event);
}

void Input::Callback::MouseMove(GLFWwindow *window, double x, double y) {

	Input::Event event;
	event.type = Input::EventType::MouseMove;
	event.mouseMove.deltaX = state.cursorPos.x - x;
	event.mouseMove.deltaY = state.cursorPos.y - y;

	state.cursorPos.x = x;
	state.cursorPos.y = y;

	state.inputEvent(event);
}

void Input::Callback::Scroll(GLFWwindow *window, double xOffset, double yOffset) {
	Input::Event event;
	event.type = Input::EventType::Scroll;
	event.scroll.xOffset = xOffset;
	event.scroll.yOffset = yOffset;

	state.lastMouseScroll.x = xOffset;
	state.lastMouseScroll.y = yOffset;

	state.inputEvent(event);
}