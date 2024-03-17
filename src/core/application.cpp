#include "application.hpp"

#include "input.hpp"
#include "resource/image_texture.hpp"
#include "visual/renderer.hpp"
#include "visual/visual.hpp"

#include "math/matrix.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Application::Application(int argc, char const *argv[]) {
}

Application::~Application() {
}

void Application::Start() {
	_window.Create(1280, 720, "Sowa Engine");

	Input::InitState(&_window);
	Visual::InitState(&_window);
	Visual::Renderer::InitState();

	Input::SetActionKeys("ui_accept", {Key::Enter});
	Input::SetActionKeys("ui_exit", {Key::Escape});

	ImageTexture texture;
	texture.Load("res/image.png");
	texture.Bind(0);

	_mainViewport.Create(1280, 720);

	while (!_window.ShouldClose()) {
		Input::Poll();
		if (Input::IsActionJustPressed("ui_exit")) {
			_window.SetShouldClose();
		}

		Visual::UseViewport(&_mainViewport);

		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		Visual::Renderer::Reset();

		Visual::Renderer::PushQuad(10, 10, 100, 100, 1, 1, 0, 1, 1, texture.ID());
		Visual::Renderer::PushQuad(120, 10, 100, 100, 1, 0, 1, 1, 1, 0);
		Visual::Renderer::PushQuad(230, 10, 100, 100, 0, 1, 1, 1, 1, texture.ID());

		Visual::Renderer::PushQuad(Matrix::CalculateTransform({200.f, 200.f}, 45.f, {200.f, 200.f}), texture.ID());

		Visual::Renderer::End();

		Visual::UseViewport(nullptr);

		glClearColor(0.0f, 0.0f, 0.0f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		Visual::Renderer::DrawFullscreen(_mainViewport.GetTargetTextureID(0));

		_window.SwapBuffers();
	}
}