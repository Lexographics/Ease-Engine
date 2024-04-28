#include "application.hpp"

#include "input.hpp"

#include "resource/font.hpp"
#include "resource/image_texture.hpp"
#include "resource/mesh.hpp"

#include "visual/renderer.hpp"
#include "visual/visual.hpp"

#include "math/matrix.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

static Application *s_app = nullptr;

Application &App() {
	return *s_app;
}

Application::Application(int argc, char const *argv[]) {
	s_app = this;
}

Application::~Application() {
}

static ImageTexture texture;
static Mesh mesh;
static Font font;

void Application::Init() {
	_fs = new FolderFileSystem("example");
	_projectSettings.Load();

	_window.Create(
		_projectSettings.rendering.window.width,
		_projectSettings.rendering.window.height,
		_projectSettings.name.c_str());

	Input::InitState(&_window);
	Visual::InitState(&_window);
	Visual::Renderer::InitState();

	Input::SetActionKeys("ui_accept", {Key::Enter});
	Input::SetActionKeys("ui_exit", {Key::Escape});

	texture.Load("tile.png");
	std::cout << texture.ID() << std::endl;
	texture.Bind(0);

	mesh.Load("teapot.obj");

	font.LoadTTF("font.ttf");

	_mainViewport.Create(
		_projectSettings.rendering.viewport.width,
		_projectSettings.rendering.viewport.height);

	NodeTypeID typeid_node = _nodeDB.NewNodeType<Node>("Node");
	_currentScene = NewScene();

	Node *node = _currentScene->Create("Node");

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

	_mainViewport.Clear(0.2f, 0.2f, 0.2f, 1.f, true);

	static float f = 0.f;
	f += 0.2f;
	glEnable(GL_DEPTH_TEST);
	Visual::Renderer::DrawMesh(mesh, Matrix::CalculateTransform3D({0.f, -20.f, -50.f}, {0.f, f, 0.f}, {10.f, 10.f, 10.f}));
	glDisable(GL_DEPTH_TEST);

	Visual::Renderer::Reset();

	Visual::Renderer::PushQuad(10, 10, 100, 100, 1, 1, 1, 1, 1, texture.ID());
	Visual::Renderer::PushQuad(120, 10, 100, 100, 1, 0, 1, 1, 1, 0);
	Visual::Renderer::PushQuad(230, 10, 100, 100, 0, 1, 1, 1, 1, texture.ID());

	Visual::Renderer::DrawText("Sowa Engine", font, Matrix::CalculateTransform({20.f, 600.f}, 0.f, {1.f, 1.f}));

	Visual::Renderer::End();

	Visual::UseViewport(nullptr);

	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	Visual::Renderer::DrawFullscreen(_mainViewport.GetTargetTextureID(0));

	_window.SwapBuffers();
}

std::shared_ptr<Scene> Application::NewScene() {
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	scene->_nodeDB = &_nodeDB;

	return scene;
}
