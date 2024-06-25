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

#include "scene/node/camera2d.hpp"
#include "scene/node/node2d.hpp"
#include "scene/node/sprite2d.hpp"
#include "scene/node/text2d.hpp"

#include "servers/script_server.hpp"

#include "debug.hpp"

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

void Application::Init() {
	_fs.RegisterFileServer("res", _fs.NewFolderFileServer("res", "res"));
	_projectSettings.Load();

	_window.Create(
		_projectSettings.rendering.window.width,
		_projectSettings.rendering.window.height,
		_projectSettings.name.c_str());

	Input::InitState(&_window);
	Visual::InitState(&_window);
	_renderer.Init();
	_renderer.RegisterRenderer2D("2D", "res://shaders/sprite2d");
	_renderer.RegisterRenderer2D("Text", "res://shaders/text2d");

	_editor.Init();

	Input::SetActionKeys("ui_accept", {Key::Enter});
	Input::SetActionKeys("ui_exit", {Key::Escape});

	Input::SetActionKeys("left", {Key::A});
	Input::SetActionKeys("right", {Key::D});
	Input::SetActionKeys("up", {Key::W});
	Input::SetActionKeys("down", {Key::S});

	texture = std::make_unique<ImageTexture>();
	mesh = std::make_unique<Mesh>();

	texture->Load("res://tile.png");

	texture->Bind(0);

	mesh->Load("res://teapot.obj");

	_defaultFont.Load("res://font.ttf");

	_mainViewport.Create(
		_projectSettings.rendering.viewport.width,
		_projectSettings.rendering.viewport.height);

	NodeTypeID typeId_Node = _nodeDB.NewNodeType<Node>("Node", 0);
	NodeTypeID typeId_Node2D = _nodeDB.NewNodeType<Node2D>("Node2D", typeId_Node);
	NodeTypeID typeId_Sprite2D = _nodeDB.NewNodeType<Sprite2D>("Sprite2D", typeId_Node2D);
	NodeTypeID typeId_Text2D = _nodeDB.NewNodeType<Text2D>("Text2D", typeId_Node2D);
	NodeTypeID typeId_Camera2D = _nodeDB.NewNodeType<Camera2D>("Camera2D", typeId_Node2D);

	_editor.RegisterNodeProp(
		typeId_Node2D,
		EditorNodeProp(
			"Position",
			EditorNodePropType::Vec2,
			[](Node *node) -> void * { return reinterpret_cast<void *>(&dynamic_cast<Node2D *>(node)->Position()); }));
	_editor.RegisterNodeProp(
		typeId_Node2D,
		EditorNodeProp(
			"Rotation",
			EditorNodePropType::Float,
			[](Node *node) -> void * { return reinterpret_cast<void *>(&dynamic_cast<Node2D *>(node)->Rotation()); }));
	_editor.RegisterNodeProp(
		typeId_Node2D,
		EditorNodeProp(
			"Scale",
			EditorNodePropType::Vec2,
			[](Node *node) -> void * { return reinterpret_cast<void *>(&dynamic_cast<Node2D *>(node)->Scale()); }));

	_editor.RegisterNodeProp(
		typeId_Sprite2D,
		EditorNodeProp(
			"Texture",
			EditorNodePropType::RID,
			[](Node *node) -> void * { return reinterpret_cast<void *>(&dynamic_cast<Sprite2D *>(node)->GetTexture()); }));
	_editor.RegisterNodeProp(
		typeId_Sprite2D,
		EditorNodeProp(
			"Modulate",
			EditorNodePropType::Color,
			[](Node *node) -> void * { return reinterpret_cast<void *>(&dynamic_cast<Sprite2D *>(node)->Modulate()); }));

	_editor.RegisterNodeProp(
		typeId_Text2D,
		EditorNodeProp(
			"Text",
			EditorNodePropType::String,
			[](Node *node) -> void * { return reinterpret_cast<void *>(&dynamic_cast<Text2D *>(node)->Text()); }));
	_editor.RegisterNodeProp(
		typeId_Text2D,
		EditorNodeProp(
			"Modulate",
			EditorNodePropType::Color,
			[](Node *node) -> void * { return reinterpret_cast<void *>(&dynamic_cast<Text2D *>(node)->Modulate()); }));
	_editor.RegisterNodeProp(
		typeId_Text2D,
		EditorNodeProp(
			"Font",
			EditorNodePropType::RID,
			[](Node *node) -> void * { return reinterpret_cast<void *>(&dynamic_cast<Text2D *>(node)->GetFont()); }));

	_editor.RegisterNodeProp(
		typeId_Camera2D,
		EditorNodeProp(
			"Rotatable",
			EditorNodePropType::Bool,
			[](Node *node) -> void * { return reinterpret_cast<void *>(&dynamic_cast<Camera2D *>(node)->Rotatable()); }));

	_backgroundScene = NewScene();
	_currentScene = NewScene();
	_currentScene->LoadFromFile("res://game.sscn");
	SetCurrentScene(_currentScene);

	Input::InputEvent().append([this](Input::Event event) {
		if (event.type == Input::EventType::Key) {
			if (event.key.key == Key::F5 && event.key.action == GLFW_PRESS) {
				if (IsRunning())
					Stop();
				else
					Start();
			}
		}
	});

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

	if (IsRunning())
		_scriptServer.CallUpdate();

	_mainViewport.Clear(0.2f, 0.2f, 0.2f, 1.f, true);

	// static float f = 0.f;
	// f += 0.2f;
	// glEnable(GL_DEPTH_TEST);
	// GetRenderer().DrawMesh(*mesh, Matrix::CalculateTransform3D({0.f, -20.f, -50.f}, {0.f, f, 0.f}, {10.f, 10.f, 10.f}));
	// glDisable(GL_DEPTH_TEST);

	if (IsRunning()) {
		GetRenderer().GetRenderer2D("2D").SetProjectionMatrix(GetCurrentScene()->GetMatrix2D());
		GetRenderer().GetRenderer2D("Text").SetProjectionMatrix(GetCurrentScene()->GetMatrix2D());
	} else {
		GetRenderer().GetRenderer2D("2D").SetProjectionMatrix(_editor.GetCamera2DMatrix());
		GetRenderer().GetRenderer2D("Text").SetProjectionMatrix(_editor.GetCamera2DMatrix());
	}
	GetRenderer().BeginDraw();

	if (_currentScene)
		_currentScene->Update();

	GetRenderer().EndDraw();

	Visual::UseViewport(nullptr);

	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	GetRenderer().DrawFullscreen(_mainViewport.GetTargetTextureID(0));
	glEnable(GL_DEPTH_TEST);

	_editor.Begin();
	_editor.Update();
	_editor.End();

	_window.SwapBuffers();
}

void Application::Start() {
	if (!_currentScene) {
		Debug::Error("Start scene is not selected");
		return;
	}

	if (_isRunning)
		return;
	_isRunning = true;
	Scene::Copy(_currentScene.get(), _backgroundScene.get());

	_scriptServer.Init();
	_currentScene->Start();
	_scriptServer.CallStart();
}

void Application::Stop() {
	if (!_isRunning)
		return;
	_isRunning = false;

	Scene::Copy(_backgroundScene.get(), _currentScene.get());
}

Ref<Scene> Application::NewScene() {
	Ref<Scene> scene = MakeRef<Scene>();
	scene->_nodeDB = &_nodeDB;

	return scene;
}

Ref<Scene> Application::GetCurrentScene() {
	return _currentScene;
}

void Application::SetCurrentScene(Ref<Scene> scene) {
	_currentScene = scene;
	_onSceneChanged();
}