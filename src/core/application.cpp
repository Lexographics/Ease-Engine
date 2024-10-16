#include "application.hpp"

#include "input.hpp"

#include "resource/audio_stream.hpp"
#include "resource/font.hpp"
#include "resource/image_texture.hpp"
#include "resource/mesh.hpp"

#include "visual/renderer.hpp"
#include "visual/visual.hpp"

#include "math/matrix.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "scene/node/animatedsprite2d.hpp"
#include "scene/node/audiostreamplayer.hpp"
#include "scene/node/camera2d.hpp"
#include "scene/node/node2d.hpp"
#include "scene/node/progress_bar.hpp"
#include "scene/node/sprite2d.hpp"
#include "scene/node/text2d.hpp"

#include "servers/script_server.hpp"

#include "debug.hpp"

#ifdef SW_WEB
#include "platform/web/web.hpp"
#endif

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
	Debug::Log("Sowa Engine {}@{}", s_gitHash, s_gitBranch);
	_fs.RegisterFileServer("res", _fs.NewFolderFileServer("res", "res"));
	RegisterBuiltinData();

	_projectSettings.Load();

	_window.Create(
#ifndef SW_WEB
		_projectSettings.rendering.window.width,
		_projectSettings.rendering.window.height,
#else
		get_window_width(),
		get_window_height(),
#endif

		_projectSettings.application.name.c_str());

	Input::InitState(&_window);
	Visual::InitState(&_window);
	_renderer.Init();
	_renderer.RegisterRenderer2D("2D", "data://sprite2d.vs", "data://sprite2d.fs");
	_renderer.RegisterRenderer2D("Text", "data://text2d.vs", "data://text2d.fs");
	{
		ImageTexture texture;
		texture.SetFlip(true);
		texture.SetStorePixels(true);
		texture.Load("data://icon.png");

		GLFWimage images[1];
		images[0].width = texture.Width();
		images[0].height = texture.Height();
		images[0].pixels = reinterpret_cast<unsigned char *>(texture.Pixels());

		glfwSetWindowIcon(GetWindow()._window, 1, images);
	}

#ifdef SW_EDITOR
	_editor.Init();
#endif

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

	_defaultFont.Load("data://font.ttf");

	_mainViewport.Create(
		_projectSettings.rendering.viewport.width,
		_projectSettings.rendering.viewport.height);

	NodeTypeID typeId_Node = _nodeDB.NewNodeType<Node>("Node", 0);
	NodeTypeID typeId_Node2D = _nodeDB.NewNodeType<Node2D>("Node2D", typeId_Node);
	NodeTypeID typeId_Sprite2D = _nodeDB.NewNodeType<Sprite2D>("Sprite2D", typeId_Node2D);
	NodeTypeID typeId_Text2D = _nodeDB.NewNodeType<Text2D>("Text2D", typeId_Node2D);
	NodeTypeID typeId_Camera2D = _nodeDB.NewNodeType<Camera2D>("Camera2D", typeId_Node2D);
	NodeTypeID typeId_AnimatedSprite2D = _nodeDB.NewNodeType<AnimatedSprite2D>("AnimatedSprite2D", typeId_Node2D);
	NodeTypeID typeId_ProgressBar = _nodeDB.NewNodeType<ProgressBar>("ProgressBar", typeId_Node2D);
	NodeTypeID typeId_AudioStreamPlayer = _nodeDB.NewNodeType<AudioStreamPlayer>("AudioStreamPlayer", typeId_Node);

	GetResourceRegistry().AddResourceType<ImageTexture>("ImageTexture");
	GetResourceRegistry().AddResourceType<Font>("Font");
	GetResourceRegistry().AddResourceType<SpriteSheetAnimation>("SpriteSheetAnimation");
	GetResourceRegistry().AddResourceType<Mesh>("Mesh");
	GetResourceRegistry().AddResourceType<AudioStream>("AudioStream");

	_backgroundScene = NewScene();
	_currentScene = NewScene();
	_currentScene->LoadFromFile(_projectSettings.application.mainScene.c_str());
	SetCurrentScene(_currentScene);

#ifdef SW_EDITOR
	Input::InputEvent().append([this](Input::Event event) {
		if (event.type == Input::EventType::Key) {
			if (event.key.key == Key::F5 && event.key.action == GLFW_PRESS) {
				if (IsRunning()) {
					Stop();
					RestoreScene();
				} else {
					StoreScene();
					GetCurrentScene()->SaveToFile();
					GetCurrentScene()->LoadFromFile(this->_projectSettings.application.mainScene.c_str());
					Start();
				}
			}

			if (event.key.key == Key::F6 && event.key.action == GLFW_PRESS) {
				if (IsRunning()) {
					Stop();
					RestoreScene();
				} else {
					StoreScene();
					Start();
				}
			}
		}
	});
#endif

	_scriptServer.Init();

	_lastUpdate = std::chrono::high_resolution_clock::now();

	// SetCursor("res://sprites/cursor_none.png");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifndef SW_EDITOR
	Start();
#endif

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
#ifdef SW_WEB
	if (is_page_visible() == 0) {
		RestartDeltaTime();
	}
#endif

	auto now = std::chrono::high_resolution_clock::now();
	_delta = std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastUpdate).count() / 1000.0;
	_lastUpdate = now;

	if (_sceneWillLoad) {
		Ref<Scene> scene = NewScene();
		if (scene->LoadFromFile(_sceneToLoad.c_str())) {
			SetCurrentScene(scene);
		} else {
			Debug::Error("Failed to load scene: {}", _sceneToLoad);
		}
		_sceneToLoad = "";
		_sceneWillLoad = false;

		_lastUpdate = std::chrono::high_resolution_clock::now();
	}

	Visual::UseViewport(&_mainViewport);

	if (IsRunning()) {
		_scriptServer.CallUpdate();

		for (auto &timer : _timers) {
			timer.Update(Delta());
		}
		for (auto &tween : _tweens) {
			tween.Update(Delta());
		}
	}

	_mainViewport.Clear(0.2f, 0.2f, 0.2f, 1.f, true);

	// static float f = 0.f;
	// f += 0.2f;
	// glEnable(GL_DEPTH_TEST);
	// GetRenderer().DrawMesh(*mesh, Matrix::CalculateTransform3D({0.f, -20.f, -50.f}, {0.f, f, 0.f}, {10.f, 10.f, 10.f}));
	// glDisable(GL_DEPTH_TEST);

	if (IsRunning()) {
		GetRenderer().GetRenderer2D("2D").SetProjectionMatrix(GetCurrentScene()->GetMatrix2D());
		GetRenderer().GetRenderer2D("Text").SetProjectionMatrix(GetCurrentScene()->GetMatrix2D());
	}
#ifdef SW_EDITOR
	else {
		GetRenderer().GetRenderer2D("2D").SetProjectionMatrix(_editor.GetCamera2DMatrix());
		GetRenderer().GetRenderer2D("Text").SetProjectionMatrix(_editor.GetCamera2DMatrix());
	}
#endif
	GetRenderer().BeginDraw();

#ifdef SW_EDITOR
	if (!IsRunning()) {
		if (_currentScene) {
			float thickness = 4 * GetEditor()._cameraZoom;

			for (Node *node : _currentScene->_nodeIter) {
				Camera2D *camera = dynamic_cast<Camera2D *>(node);
				if (!camera)
					continue;

				// TODO: GetBounds does not calculate rotation
				Rect bounds = camera->GetBounds();
				GetRenderer().GetRenderer2D("2D").DrawLine({bounds.x, bounds.y}, {bounds.x + bounds.w, bounds.y}, thickness, Color::RGB(20, 110, 190, 170));
				GetRenderer().GetRenderer2D("2D").DrawLine({bounds.x + bounds.w, bounds.y}, {bounds.x + bounds.w, bounds.y + bounds.h}, thickness, Color::RGB(20, 110, 190, 170));
				GetRenderer().GetRenderer2D("2D").DrawLine({bounds.x + bounds.w, bounds.y + bounds.h}, {bounds.x, bounds.y + bounds.h}, thickness, Color::RGB(20, 110, 190, 170));
				GetRenderer().GetRenderer2D("2D").DrawLine({bounds.x, bounds.y + bounds.h}, {bounds.x, bounds.y}, thickness, Color::RGB(20, 110, 190, 170));
			}

			Node *selectedNode = GetCurrentScene()->GetNode(GetEditor()._selectedNodeID);
			if (Node2D *selectedNode2D = dynamic_cast<Node2D *>(selectedNode); selectedNode2D != nullptr) {
				Vector2 pos = selectedNode2D->GetGlobalPosition();
				float width = 120 * GetEditor()._cameraZoom / 2;
				float thickness = 6 * GetEditor()._cameraZoom;

				GetRenderer().GetRenderer2D("2D").DrawLine({pos.x - width, pos.y}, {pos.x + width, pos.y}, thickness, Color::RGB(110, 120, 220, 170));
				GetRenderer().GetRenderer2D("2D").DrawLine({pos.x, pos.y - width}, {pos.x, pos.y + width}, thickness, Color::RGB(110, 120, 220, 170));
			}
		}
	}
#endif

	if (_currentScene)
		_currentScene->Update();

#ifdef SW_EDITOR
	if (!IsRunning()) {
		float thickness = 4 * GetEditor()._cameraZoom;

		GetRenderer().GetRenderer2D("2D").DrawLine({0.f, 0.f}, {1920.f * 100, 0.f}, thickness, Color::RGB(200, 120, 100, 170));
		GetRenderer().GetRenderer2D("2D").DrawLine({0.f, 0.f}, {0.f, 1080.f * 100}, thickness, Color::RGB(120, 255, 100, 170));

		Vector2 videoSize = Vector2(
			App().GetProjectSettings().rendering.viewport.width,
			App().GetProjectSettings().rendering.viewport.height);

		GetRenderer().GetRenderer2D("2D").DrawLine({0.f, 0.f}, {videoSize.x, 0.f}, thickness, Color::RGB(20, 130, 220, 140));
		GetRenderer().GetRenderer2D("2D").DrawLine({videoSize.x, 0.f}, {videoSize.x, videoSize.y}, thickness, Color::RGB(20, 130, 220, 140));
		GetRenderer().GetRenderer2D("2D").DrawLine({videoSize.x, videoSize.y}, {0.f, videoSize.y}, thickness, Color::RGB(20, 130, 220, 140));
		GetRenderer().GetRenderer2D("2D").DrawLine({0.f, videoSize.y}, {0.f, 0.f}, thickness, Color::RGB(20, 130, 220, 140));
	}
#endif

#ifdef SW_EDITOR
	if (!IsRunning()) {
		Vector2 center = _editor._camera2d.Position();
		Vector2 size = Vector2(1920.f, 1080.f) * _editor._cameraZoom;

		float logZoom = log10f(_editor._cameraZoom * 10) * 2;
		logZoom = std::max(logZoom, 0.1f);

		float spacing = 16 * std::pow(3, std::ceil(logZoom));
		spacing = std::max(spacing, 16.f);

		for (float x = (((int)(center.x - size.x * 0.5f) / (int)spacing) * spacing) - spacing; x <= (center.x + size.x * 0.5f) + spacing; x += spacing) {
			GetRenderer().GetRenderer2D("2D").DrawLine(glm::vec2(x, center.y - size.y), glm::vec2(x, center.y + size.y), 3.f * _editor._cameraZoom, Color::RGBAFloat(1.f, 1.f, 1.f, 0.2f));

			GetRenderer().GetRenderer2D("2D").DrawLine(glm::vec2(x + spacing * 0.25f, center.y - size.y), glm::vec2(x + spacing * 0.25f, center.y + size.y), 2.f * _editor._cameraZoom, Color::RGBAFloat(1.f, 1.f, 1.f, 0.1f));
			GetRenderer().GetRenderer2D("2D").DrawLine(glm::vec2(x + spacing * 0.50f, center.y - size.y), glm::vec2(x + spacing * 0.50f, center.y + size.y), 2.f * _editor._cameraZoom, Color::RGBAFloat(1.f, 1.f, 1.f, 0.1f));
			GetRenderer().GetRenderer2D("2D").DrawLine(glm::vec2(x + spacing * 0.75f, center.y - size.y), glm::vec2(x + spacing * 0.75f, center.y + size.y), 2.f * _editor._cameraZoom, Color::RGBAFloat(1.f, 1.f, 1.f, 0.1f));

			for (float y = (((int)(center.y - size.y * 0.5f) / (int)spacing) * spacing) - spacing; y <= (center.y + size.y * 0.5f) + spacing; y += spacing) {
				GetRenderer().GetRenderer2D("2D").DrawLine(glm::vec2(center.x - size.x, y), glm::vec2(center.x + size.x, y), 3.f * _editor._cameraZoom, Color::RGBAFloat(1.f, 1.f, 1.f, 0.2f));

				GetRenderer().GetRenderer2D("2D").DrawLine(glm::vec2(center.x - size.x, y + spacing * 0.25f), glm::vec2(center.x + size.x, y + spacing * 0.25f), 2.f * _editor._cameraZoom, Color::RGBAFloat(1.f, 1.f, 1.f, 0.1f));
				GetRenderer().GetRenderer2D("2D").DrawLine(glm::vec2(center.x - size.x, y + spacing * 0.50f), glm::vec2(center.x + size.x, y + spacing * 0.50f), 2.f * _editor._cameraZoom, Color::RGBAFloat(1.f, 1.f, 1.f, 0.1f));
				GetRenderer().GetRenderer2D("2D").DrawLine(glm::vec2(center.x - size.x, y + spacing * 0.75f), glm::vec2(center.x + size.x, y + spacing * 0.75f), 2.f * _editor._cameraZoom, Color::RGBAFloat(1.f, 1.f, 1.f, 0.1f));
			}
		}
	}
#endif

	GetRenderer().EndDraw();

	Vector2 mousePos = Input::GetMousePosition();
	_hoveredID = static_cast<NodeID>(_mainViewport.ReadAttachmentInt(1, mousePos.x, GetProjectSettings().rendering.viewport.height - mousePos.y));

	Visual::UseViewport(nullptr, &_viewportRect);

	glClearColor(0.0f, 0.0f, 0.0f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	GetRenderer().DrawFullscreen(_mainViewport.GetTargetTextureID(0));
	glEnable(GL_DEPTH_TEST);

#ifdef SW_EDITOR
	_editor.Begin();
	_editor.Update();
	_editor.End();
#endif

	_window.SwapBuffers();
	Input::Poll();
}

void Application::Start() {
	if (!_currentScene) {
		Debug::Error("Start scene is not selected");
		return;
	}

	if (_isRunning)
		return;
	_isRunning = true;
	_copyGlobalStore = _globalStore;

	_currentScene->Start();
	_scriptServer.CallStart();
}

void Application::Stop() {
	if (!_isRunning)
		return;
	_isRunning = false;

	_currentScene->Shutdown();
	_timers.clear();
	_tweens.clear();

	_globalStore = _copyGlobalStore;
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
	if (_currentScene && IsRunning()) {
		_currentScene->Shutdown();
		_timers.clear();
		_tweens.clear();
	}
	_currentScene = scene;
	if (IsRunning()) {
		_currentScene->Start();
	}
	_onSceneChanged();
}

void Application::LoadScene(const std::string &path) {
	_sceneWillLoad = true;
	_sceneToLoad = path;
}

void Application::SetCursor(const std::string &texturePath) {
	// TODO: Implement on web
#ifndef SW_WEB
	ImageTexture texture;
	texture.SetStorePixels(true);
	texture.SetFlip(true);
	texture.Load(texturePath.c_str());

	GLFWimage image;
	image.width = texture.Width();
	image.height = texture.Height();
	image.pixels = (unsigned char *)texture.Pixels();

	GLFWcursor *cursor = glfwCreateCursor(&image, 10, 6);
	if (!cursor) {
		Debug::Error("Failed to load cursor: {}", texturePath);
		return;
	}

	glfwSetCursor(GetWindow()._window, cursor);
#endif
}

void Application::RestartDeltaTime() {
	_lastUpdate = std::chrono::high_resolution_clock::now();
}

void Application::StoreScene() {
	Scene::Copy(_currentScene.get(), _backgroundScene.get());
}
void Application::RestoreScene() {
	Scene::Copy(_backgroundScene.get(), _currentScene.get());
}

extern "C" void onVisibilityChange(int visibilityState) {
	if (visibilityState == 1) {
		App().RestartDeltaTime();
	}
}

extern "C" void onWindowResize(float width, float height) {
	App().GetWindow().SetWindowSize(Vector2(width, height));
}