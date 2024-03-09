#include "application.hpp"

#include "input.hpp"

Application::Application(int argc, char const *argv[]) {
}

Application::~Application() {
}

void Application::Start() {
	Input::InitState(&_window);

	_window.Create(1280, 720, "Sowa Engine");

	while (!_window.ShouldClose()) {
		_window.SwapBuffers();

		_window.PollEvents();
		if (Input::IsKeyDown(Key::Escape)) {
			_window.SetShouldClose();
		}
	}
}