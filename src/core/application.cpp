#include "application.hpp"

Application::Application(int argc, char const *argv[]) {
}

Application::~Application() {
}

void Application::Start() {
	_window.Create(1920, 1080, "Sowa Engine");

	while (!_window.ShouldClose()) {
		_window.SwapBuffers();

		_window.PollEvents();
	}
}