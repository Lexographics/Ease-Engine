#include "application.hpp"

#include "file.hpp"
#include "input.hpp"

#include "resource/image_texture.hpp"
#include "resource/mesh.hpp"

#include "visual/renderer.hpp"
#include "visual/visual.hpp"

#include "math/matrix.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Application::Application(int argc, char const *argv[]) {
}

Application::~Application() {
}

static ImageTexture texture;
static Mesh mesh;

void Application::Init() {

	_window.Create(1280, 720, "Sowa Engine");

	Input::InitState(&_window);
	Visual::InitState(&_window);
	Visual::Renderer::InitState();
	File::InitState();

	Input::SetActionKeys("ui_accept", {Key::Enter});
	Input::SetActionKeys("ui_exit", {Key::Escape});

	texture.Load("res/image.png");
	texture.Bind(0);

	mesh.Load("res/teapot.obj");

	_mainViewport.Create(1280, 720);

#ifdef SW_WEB
	emscripten_set_main_loop_arg(
		[](void *self) {
			static_cast<Application *>(self)->Update();
		},
		this, 0, 1);
#else
	while (!_window.ShouldClose()) {
		Update();
	}
#endif
}

void Application::Update() {
	Input::Poll();
	if (Input::IsActionJustPressed("ui_exit")) {
		_window.SetShouldClose();
	}

	Visual::UseViewport(&_mainViewport);

	glClearColor(0.2f, 0.2f, 0.2f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Visual::Renderer::Reset();

	Visual::Renderer::PushQuad(10, 10, 100, 100, 1, 1, 0, 1, 1, texture.ID());
	Visual::Renderer::PushQuad(120, 10, 100, 100, 1, 0, 1, 1, 1, 0);
	Visual::Renderer::PushQuad(230, 10, 100, 100, 0, 1, 1, 1, 1, texture.ID());

	Visual::Renderer::PushQuad(Matrix::CalculateTransform({200.f, 200.f}, 45.f, {200.f, 200.f}), texture.ID());

	Visual::Renderer::End();

	static float f = 0.f;
	f += 0.2f;

	glEnable(GL_DEPTH_TEST);
	Visual::Renderer::DrawMesh(mesh, Matrix::CalculateTransform3D({0.f, -20.f, -50.f}, {0.f, f, 0.f}, {10.f, 10.f, 10.f}));
	glDisable(GL_DEPTH_TEST);

	Visual::UseViewport(nullptr);

	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	Visual::Renderer::DrawFullscreen(_mainViewport.GetTargetTextureID(0));

	_window.SwapBuffers();
}