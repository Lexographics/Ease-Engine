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

#include "scene/node/node2d.hpp"
#include "scene/node/sprite2d.hpp"
#include "scene/node/text2d.hpp"

static Application *s_app = nullptr;

Application &App() {
	return *s_app;
}

Application::Application(int argc, char const *argv[]) {
	s_app = this;
}

Application::~Application() {
}

std::unique_ptr<ImageTexture> texture;
std::unique_ptr<Mesh> mesh;

Sprite2D *ssss;
Text2D *ss;

void Application::Init() {
	_fs = new FolderFileSystem("res");
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

	texture = std::make_unique<ImageTexture>();
	mesh = std::make_unique<Mesh>();

	texture->Load("tile.png");
	GetResourceRegistry().AddResource(texture.get());

	std::cout << texture->ID() << std::endl;
	texture->Bind(0);

	mesh->Load("teapot.obj");

	_defaultFont.LoadTTF("font.ttf");

	_mainViewport.Create(
		_projectSettings.rendering.viewport.width,
		_projectSettings.rendering.viewport.height);

	_nodeDB.NewNodeType<Node>("Node");
	_nodeDB.NewNodeType<Node2D>("Node2D");
	_nodeDB.NewNodeType<Sprite2D>("Sprite2D");
	_nodeDB.NewNodeType<Text2D>("Text2D");
	_currentScene = NewScene();

	Node *node = _currentScene->Create("Node");
	Node *child1 = _currentScene->Create("Text2D", "Child1");
	Node *child1child1 = _currentScene->Create("Sprite2D", "Child1Child1");
	Node *child2 = _currentScene->Create("Node", "Child2");
	Node *child2child1 = _currentScene->Create("Node", "Child2Child1");

	node->AddChild(child1);
	node->AddChild(child2);
	child2->AddChild(child2child1);

	std::cout << node->GetNode("Child2/Child2Child1")->Name() << std::endl;

	node->GetNode("Child1")->AddChild(child1child1);

	ssss = dynamic_cast<Sprite2D *>(node->GetNode("Child1/Child1Child1"));
	ssss->Position() = {0.f, 0.f};
	ssss->Scale() = {.06f, .06f};
	ssss->GetTexture() = texture->GetRID();

	ss = dynamic_cast<Text2D *>(node->GetNode("Child1"));
	ss->Text() = "This is a text";

	node->PrintHierarchy();

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
	Visual::Renderer::DrawMesh(*mesh, Matrix::CalculateTransform3D({0.f, -20.f, -50.f}, {0.f, f, 0.f}, {10.f, 10.f, 10.f}));
	glDisable(GL_DEPTH_TEST);

	Visual::Renderer::Reset();

	ss->Position() = {300.f, 400.f};
	ss->Rotation() += 0.6f;
	ssss->Rotation() += 0.2f;

	_currentScene->Update();

	Visual::Renderer::PushQuad(10, 10, 100, 100, 1, 1, 1, 1, 1, texture->ID());
	Visual::Renderer::PushQuad(120, 10, 100, 100, 1, 0, 1, 1, 1, 0);
	Visual::Renderer::PushQuad(230, 10, 100, 100, 0, 1, 1, 1, 1, texture->ID());

	Visual::Renderer::DrawText("Sowa Engine", _defaultFont, Matrix::CalculateTransform({20.f, 600.f}, 0.f, {1.f, 1.f}));

	Visual::Renderer::End();

	Visual::UseViewport(nullptr);

	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	Visual::Renderer::DrawFullscreen(_mainViewport.GetTargetTextureID(0));

	_window.SwapBuffers();
}

Ref<Scene> Application::NewScene() {
	Ref<Scene> scene = MakeRef<Scene>();
	scene->_nodeDB = &_nodeDB;

	return scene;
}
