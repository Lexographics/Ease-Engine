#include "editor.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"

#include "core/application.hpp"
#include "core/debug.hpp"
#include "core/filesystem/filesystem.hpp"

#include "math/matrix.hpp"
#include "scene/node/camera2d.hpp"

#include "gui.hpp"
#include "resource/sprite_sheet_animation.hpp"

void Editor::Init() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(App().GetWindow()._window, true);
	// ImGui_ImplOpenGL3_Init("#version 130");
	ImGui_ImplOpenGL3_Init("#version 100");

	Ref<FileData> guiData = App().FS().Load("data://imgui.ini");
	if (guiData) {
		io.IniFilename = NULL;
		ImGui::LoadIniSettingsFromMemory((const char *)guiData->Data(), guiData->Size());
	}

	Ref<FileData> font = App().FS().Load("data://font.ttf");
	if (font) {
		io.Fonts->AddFontFromMemoryTTF(font->Data(), font->Size(), 18, nullptr, io.Fonts->GetGlyphRangesKorean());
		io.Fonts->Build();
	}

	ImGuiStyle &style = ImGui::GetStyle();
	ImVec4 *colors = ImGui::GetStyle().Colors;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.39f, 0.39f, 0.39f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.43f, 0.43f, 0.43f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.57f, 0.57f, 0.57f, 0.54f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.37f, 0.62f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.53f, 0.53f, 0.53f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.51f, 0.51f, 0.51f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.62f, 0.62f, 0.62f, 0.40f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.62f, 0.62f, 0.62f, 0.40f);
	colors[ImGuiCol_Header] = ImVec4(0.48f, 0.48f, 0.48f, 0.40f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.62f, 0.62f, 0.62f, 0.40f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.64f, 0.64f, 0.64f, 0.40f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.37f, 0.37f, 0.37f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	Input::InputEvent().append([this](Input::Event event) {
		if (event.type == Input::EventType::MouseMove) {
			if (!App().IsRunning() && this->_cameraDragging) {
				this->_camera2d.Position().x += event.mouseMove.deltaX * this->_cameraZoom;
				this->_camera2d.Position().y -= event.mouseMove.deltaY * this->_cameraZoom;
			}
		}
	});

	_fileLeftClickEvent[".sscn"] = [this](std::filesystem::path path) {
		for (size_t i = 0; i < this->_scenes.size(); i++) {
			if (this->_scenes[i]->GetFilepath().string() == path.string()) {
				App().SetCurrentScene(this->_scenes[i]);
				return;
			}
		}

		Ref<Scene> scene = _scenes.emplace_back(App().NewScene());
		scene->LoadFromFile(path.string().c_str());

		this->_ignoreOnSceneChanged = true;
		App().SetCurrentScene(scene);
		this->_ignoreOnSceneChanged = false;
	};

	_fileContextMenu[".png"] = [this](std::filesystem::path path) {
		if (ImGui::MenuItem("Import to scene")) {
			ImageTexture *texture = App().GetResourceRegistry().NewResource<ImageTexture>();
			texture->Load(path.string().c_str());
			App().GetResourceRegistry().AddResource(texture);
		}
	};

	_fileContextMenu[".lua"] = [this](std::filesystem::path path) {
		if (ImGui::MenuItem("Add to scene")) {
			App().GetCurrentScene()->_scripts.push_back(path.string());
		}
	};

	App().OnSceneChanged([this]() {
		if (this->_ignoreOnSceneChanged || App().GetCurrentScene() == nullptr)
			return;

		for (size_t i = 0; i < this->_scenes.size(); i++) {
			if (this->_scenes[i]->GetFilepath().string() == App().GetCurrentScene()->GetFilepath()) {
				return;
			}
		}
		_scenes.push_back(App().GetCurrentScene());
	});
}

void Editor::Begin() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Editor::Update() {
	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoCollapse;

	glm::vec2 winSize = App().GetWindow().GetWindowSize();
	ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(winSize.x, winSize.y), ImGuiCond_Always);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
	ImGui::Begin("Editor", nullptr,
				 windowFlags | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
	ImGui::PopStyleVar();

	ImGui::DockSpace(ImGui::GetID("Editor"), ImVec2(0.f, 0.f), ImGuiDockNodeFlags_PassthruCentralNode);

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu(" File ")) {
			if (ImGui::MenuItem("Save current scene")) {
				App().GetCurrentScene()->SaveToFile(nullptr);
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(" Edit ")) {
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(" Run ")) {
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	ImGui::Begin("Console");

	static bool at_bottom = true;
	for (auto &line : Debug::GetLines()) {
		ImVec4 color = ImColor{IM_COL32_WHITE};
		if (line.severity == Debug::LogSeverity::Log) {
			color = ImColor{IM_COL32(173, 216, 230, 255)};
		} else if (line.severity == Debug::LogSeverity::Info) {
			color = ImColor{IM_COL32(0, 255, 255, 255)};
		} else if (line.severity == Debug::LogSeverity::Warn) {
			color = ImColor{IM_COL32(255, 255, 0, 255)};
		} else if (line.severity == Debug::LogSeverity::Error) {
			color = ImColor{IM_COL32(255, 0, 0, 255)};
		}

		ImGui::PushStyleColor(ImGuiCol_Text, color);
		ImGui::Text("%s", line.message.c_str());
		ImGui::PopStyleColor();
	}

	if (ImGui::GetScrollY() > ImGui::GetScrollMaxY() - 0.5f) {
		at_bottom = true;
	}

	if ((ImGui::IsWindowHovered() && ImGui::GetIO().MouseWheel > 0) || ImGui::GetScrollY() < ImGui::GetScrollMaxY()) {
		at_bottom = false;
	}

	if (at_bottom) {
		ImGui::SetScrollHereY(1.f);
	}

	ImGui::End();

	if (_currentAnimation != 0) {
		SpriteSheetAnimation *anim = dynamic_cast<SpriteSheetAnimation *>(App().GetResourceRegistry().GetResource(_currentAnimation));
		if (anim) {
			if (ImGui::Begin("Animation")) {

				ImGui::BeginTable("##Table", 2, ImGuiTableFlags_Resizable);
				ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, ImGui::GetWindowSize().x * 0.2f);
				ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);
				ImGui::TableNextColumn();

				auto anims = anim->GetAnimations();

				std::string animationToDelete = "";
				bool animationDeleted = false;
				for (auto &[name, sheet] : anims) {
					ImGui::PushID(name.c_str());
					if (_currentAnimName == "") {
						_currentAnimName = name;
					}

					ImGui::Text("%s", name.c_str());
					ImGui::SameLine();
					if (ImGui::Button("Edit")) {
						_currentAnimName = name;
					}
					ImGui::SameLine();
					if (ImGui::Button("Delete")) {
						animationToDelete = name;
						animationDeleted = true;
					}

					ImGui::PopID();
				}
				if (animationDeleted) {
					anim->RemoveAnimation(animationToDelete);
				}

				if (ImGui::Button("Create New")) {
					ImGui::OpenPopup("Animation_CreateNew");
				}

				if (ImGui::BeginPopup("Animation_CreateNew")) {
					std::string name;
					if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue)) {
						anim->SetAnimation(name, SpriteSheet());
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}

				ImGui::TableNextColumn();

				if (_currentAnimName != "" && anim->HasAnimation(_currentAnimName)) {
					SpriteSheet *sheet = anim->GetAnimation(_currentAnimName);

					ImGui::Text("Animation: %s", _currentAnimName.c_str());
					Gui::TexturePicker("##Texture", sheet->texture);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(120.f);
					ImGui::InputInt2("Frame Count", &sheet->gridSize.x);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(120.f);
					ImGui::DragFloat("Speed", &sheet->speed, 0.1f, 0.f, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp);

					ImageTexture *tex = dynamic_cast<ImageTexture *>(App().GetResourceRegistry().GetResource(sheet->texture));
					if (tex) {
						static float textureScale = 1.f;
						ImGui::SliderFloat("Scale", &textureScale, 0.1f, 10.f);

						// ImGui::Image((ImTextureID)(intptr_t)tex->ID(), ImVec2(tex->Width() * textureScale, tex->Height() * textureScale), ImVec2(0, 1), ImVec2(1, 0));

						ImVec2 uvSize = ImVec2(1.f / sheet->gridSize.x, 1.f / sheet->gridSize.y);

						for (int y = 0; y < sheet->gridSize.y; y++) {
							ImGui::PushID(y);
							for (int x = 0; x < sheet->gridSize.x; x++) {
								ImGui::PushID(x);
								int invertedY = sheet->gridSize.y - y - 1;

								auto it = std::find(sheet->frames.begin(), sheet->frames.end(), glm::ivec2(x, y));
								if (it != sheet->frames.end()) {
									ImGui::PushStyleColor(ImGuiCol_Button, (ImU32)ImColor(0.1f, 0.1f, 0.8f, 1.f));
								} else {
									ImGui::PushStyleColor(ImGuiCol_Button, (ImU32)ImColor(0.1f, 0.1f, 0.1f, 1.f));
								}

								ImVec2 imageTopLeft = ImGui::GetWindowPos();
								imageTopLeft.x += ImGui::GetCursorPos().x;
								imageTopLeft.y += ImGui::GetCursorPos().y;

								imageTopLeft.x -= ImGui::GetScrollX();
								imageTopLeft.y -= ImGui::GetScrollY();

								if (ImGui::ImageButton(
										"Image",
										(ImTextureID)(intptr_t)tex->ID(),
										ImVec2(
											tex->Width() * textureScale / sheet->gridSize.x,
											tex->Height() * textureScale / sheet->gridSize.y),

										ImVec2(x * uvSize.x, (invertedY * uvSize.y) + uvSize.y),
										ImVec2((x * uvSize.x) + uvSize.x, (invertedY * uvSize.y))
										// ImVec2((x * (sheet->gridSize.x + 1) * uvSize.x) + uvSize.x, (y * sheet->gridSize.y * uvSize.y) + uvSize.y)
										// ImVec2(1, 0)
										)) {

									auto it = std::find(sheet->frames.begin(), sheet->frames.end(), glm::ivec2(x, y));
									if (it != sheet->frames.end()) {
										sheet->frames.erase(it);
									} else {
										sheet->frames.push_back(glm::ivec2(x, y));
									}
								}

								if (it != sheet->frames.end()) {
									ImGui::GetWindowDrawList()->AddText(imageTopLeft, 0xFFFFFFFF, std::to_string(static_cast<int>(it - sheet->frames.begin())).c_str());
								}

								ImGui::PopStyleColor();

								if (x + 1 < sheet->gridSize.x)
									ImGui::SameLine();

								ImGui::PopID();
							}
							ImGui::PopID();
						}
					}
				}

				ImGui::EndTable();
			}
			ImGui::End();
		}
	}

	ImGui::Begin("Filesystem");

	bool openFileContext = false;
	std::function<void(std::filesystem::path path)> drawDir;
	drawDir = [&drawDir, this, &openFileContext](std::filesystem::path path) {
		auto dir = App().FS().ReadDirectory(path);

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanFullWidth;

		for (auto &entry : dir) {
			if (!entry.is_directory)
				continue;

			if (ImGui::TreeNodeEx(entry.path.filename().string().c_str(), flags)) {
				drawDir(Utils::Format("{}/{}", path, entry.path.filename()));
				ImGui::TreePop();
			}
		}

		for (auto &entry : dir) {
			if (entry.is_directory)
				continue;

			if (ImGui::TreeNodeEx(entry.path.filename().string().c_str(), flags | ImGuiTreeNodeFlags_Leaf)) {
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
					auto fn = _fileLeftClickEvent[entry.path.extension().string()];
					if (fn) {
						fn(entry.path);
					}
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
					this->_currentRClickPath = entry;
					openFileContext = true;
				}

				ImGui::TreePop();
			}
		}
	};
	drawDir("res://");

	if (openFileContext) {
		ImGui::OpenPopup("##FileRclick");
	}

	if (ImGui::BeginPopup("##FileRclick")) {
		if (ImGui::MenuItem("Delete")) {
			Debug::Log("Deleted file");
		}

		auto fn = _fileContextMenu[_currentRClickPath.path.extension().string()];
		if (fn) {
			fn(_currentRClickPath.path);
		}

		ImGui::EndPopup();
	}

	ImGui::End();

	ImGui::Begin("Scene");
	Ref<Scene> scene = App().GetCurrentScene();
	if (scene) {
		std::function<void(Node *)> drawNode;

		bool sceneNodeRclick = false;

		drawNode = [this, &drawNode, &sceneNodeRclick](Node *node) -> void {
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
			if (nullptr == node->GetParent())
				flags |= ImGuiTreeNodeFlags_DefaultOpen;

			if (node->GetChildCount() == 0)
				flags |= ImGuiTreeNodeFlags_Leaf;

			ImGui::PushID(std::to_string(node->ID()).c_str());

			if (node->ID() == this->_selectedNodeID) {
				flags |= ImGuiTreeNodeFlags_Selected;
			}
			bool open = ImGui::TreeNodeEx(node->Name().c_str(), flags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
				this->_selectedNodeID = node->ID();
			}
			if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
				sceneNodeRclick = true;
				this->_selectedNodeID = node->ID();
			}

			if (open) {
				for (Node *child : node->GetChildren()) {
					drawNode(child);
				}

				ImGui::TreePop();
			}

			ImGui::PopID();
		};
		Node *root = scene->GetRoot();
		if (root)
			drawNode(root);

		if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			_selectedNodeID = 0;
		}

		if (!root && ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
			_selectedNodeID = 0;
			sceneNodeRclick = true;
		}

		if (sceneNodeRclick)
			ImGui::OpenPopup("Scene__Node_RClick");

		if (ImGui::BeginPopup("Scene__Node_RClick")) {
			Node *selectedNode = App().GetCurrentScene()->GetNode(_selectedNodeID);
			if (selectedNode) {
				if (ImGui::Selectable("Delete Node")) {
					selectedNode->Free();
				}

				if (Camera2D *camera = dynamic_cast<Camera2D *>(selectedNode); nullptr != camera) {
					if (ImGui::Selectable("Make Current")) {
						camera->MakeCurrent();
					}
				}
			}

			if (ImGui::BeginMenu("Create New")) {
				for (auto &[typeId, nodeType] : App().GetNodeDB().GetNodeTypes())
					if (ImGui::Selectable(nodeType.name.c_str())) {
						Node *node = App().GetCurrentScene()->Create(typeId, "New Node");
						if (selectedNode) {
							selectedNode->AddChild(node);
						} else {
							App().GetCurrentScene()->SetRoot(node);
						}
						_selectedNodeID = node->ID();
					}

				ImGui::EndMenu();
			}

			ImGui::EndPopup();
		}
	}
	ImGui::End();

	ImGui::Begin("Properties");

	if (_selectedNodeID != 0) {
		Node *node = App().GetCurrentScene()->GetNode(_selectedNodeID);
		if (!node) {
			_selectedNodeID = 0;
		} else {
			ImGui::TextDisabled("%s", App().GetNodeDB().GetNodeTypename(node->TypeID()));
			ImGui::TextDisabled("ID: %s", std::to_string(node->ID()).c_str());
			if (ImGui::IsItemHovered()) {
				ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			}
			if (ImGui::IsItemClicked()) {
				Debug::Log("Node ID: {} (this should be copied to clipboard)", node->ID());
			}

			std::string name = node->Name();
			if (ImGui::InputText("Name", &name, ImGuiInputTextFlags_EnterReturnsTrue)) {
				node->Rename(name);
			}

			ImGui::PushID(node->ID());
			node->UpdateEditor();
			ImGui::PopID();
		}
	}

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();

	if (ImGui::CollapsingHeader("Editor", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();

		for (size_t i = 0; i < _scenes.size(); i++) {
			ImGui::PushID((void *)_scenes[i].get());

			ImGui::Text("%s", _scenes[i]->GetFilepath().c_str());
			ImGui::SameLine();

			if (ImGui::Button("Open")) {
				App().SetCurrentScene(_scenes[i]);
			}

			ImGui::PopID();
		}

		ImGui::Unindent();
	}

	if (ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();

		if (scene) {
			Camera2D *camera = dynamic_cast<Camera2D *>(scene->GetNode(scene->GetCurrentCamera2D()));
			if (camera) {
				ImGui::Text("2D Camera: \"%s\"", camera->Name().c_str());

				ImGui::SameLine();
				if (ImGui::Button("Clear")) {
					scene->SetCurrentCamera2D(0);
				}
			} else
				ImGui::Text("2D Camera: <NONE>");

			if (ImGui::CollapsingHeader("Scripts", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Indent();

				for (size_t i = 0; i < scene->_scripts.size();) {
					ImGui::PushID((void *)&scene->_scripts[i]);

					ImGui::Text("%s", scene->_scripts[i].c_str());
					ImGui::SameLine();
					if (ImGui::Button("x", ImVec2(24, 24))) {
						scene->_scripts.erase(scene->_scripts.begin() + i);
					} else {
						i++;
					}

					ImGui::PopID();
				}

				if (ImGui::Button("+", ImVec2(24, 24))) {
					ImGui::OpenPopup("Scene_Scripts_Add");
				}

				ImGui::Unindent();
			}
		}

		if (ImGui::BeginPopup("Scene_Scripts_Add")) {
			std::string buf = "";
			ImGui::Text("Path");
			ImGui::SameLine();

			if (ImGui::InputText("##Path", &buf, ImGuiInputTextFlags_EnterReturnsTrue)) {
				scene->_scripts.push_back(buf);
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::Unindent();
	}

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1, 1));
	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoBackground);
	ImGui::PopStyleVar();

	ImVec2 pos;
	ImVec2 size;

	ImVec2 windowSize = ImGui::GetContentRegionAvail();

	float windowRatio = (float)windowSize.x / windowSize.y;
	float videoRatio = (float)1920.f / 1080.f;

	if (windowRatio > videoRatio) {
		float width = windowSize.y * videoRatio;
		float height = windowSize.y;
		float gap = windowSize.x - width;

		float x = gap / 2.f;

		pos.x = x;
		pos.y = 0.f;
		size.x = width;
		size.y = height;

	} else {
		float width = windowSize.x;
		float height = windowSize.x / videoRatio;
		float gap = windowSize.y - height;

		float y = gap / 2.f;

		pos.x = 0.f;
		pos.y = y;
		size.x = width;
		size.y = height;
	}

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + pos.x);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + pos.y);

	ImVec2 startPos = ImGui::GetCursorPos();
	ImGui::Image((ImTextureID) static_cast<u64>(App().GetMainViewport().GetTargetTextureID(0)), size, ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));

	if (App().IsRunning()) {
		startPos.x += ImGui::GetWindowPos().x;
		startPos.y += ImGui::GetWindowPos().y;

		ImGui::GetWindowDrawList()->AddRect(
			ImVec2(startPos.x, startPos.y),
			ImVec2(startPos.x + size.x, startPos.y + size.y),
			ImColor(10, 20, 200), 0.f, 0, 4.f);
	}

	if (!App().IsRunning()) {
		if (ImGui::IsItemHovered()) {
			_cameraZoom -= (Input::GetMouseScrollY() * 0.1);
			_cameraZoom = std::clamp(_cameraZoom, 0.1f, 10.f);
			_camera2d.Scale() = Vector2(_cameraZoom, _cameraZoom);
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
			_cameraDragging = true;
		}

		if (!ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
			_cameraDragging = false;
		}
	}

	ImGui::End();

	ImGui::End();
}

void Editor::End() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

glm::mat4 Editor::GetCamera2DMatrix() {
	return _camera2d.GetMatrix();
}