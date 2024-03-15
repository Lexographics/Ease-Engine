#include "application.hpp"

#include "input.hpp"
#include <iostream>

Application::Application(int argc, char const *argv[]) {
}

Application::~Application() {
}

void Application::Start() {
	Input::InitState(&_window);

	_window.Create(1280, 720, "Sowa Engine");

	Input::SetActionKeys("ui_accept", {Key::Enter});
	Input::SetActionKeys("ui_exit", {Key::Escape});

	while (!_window.ShouldClose()) {
		_window.SwapBuffers();

		Input::Poll();
		if (Input::IsActionJustPressed("ui_exit")) {
			_window.SetShouldClose();
		}
	}
}