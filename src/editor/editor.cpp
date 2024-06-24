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

	FileData guiData = App().FS()->Load("res://imgui.ini");
	if (guiData)
		ImGui::LoadIniSettingsFromMemory((const char *)guiData->buffer.data(), guiData->buffer.size());

	FileData font = App().FS()->Load("res://font.ttf");
	if (font) {
		io.Fonts->AddFontFromMemoryTTF(font->buffer.data(), font->buffer.size(), 17, nullptr, io.Fonts->GetGlyphRangesKorean());
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

	ImGui::Begin("Filesystem");

	ImGui::End();

	ImGui::Begin("Scene");
	Ref<Scene> scene = App().GetCurrentScene();

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

			std::function<void(Node * node, NodeTypeID nodeType)> drawNodeProps;

			drawNodeProps = [this, &drawNodeProps](Node *node, NodeTypeID nodeType) -> void {
				if (_nodeProps[nodeType].size() > 0 && ImGui::CollapsingHeader(App().GetNodeDB().GetNodeTypename(nodeType), ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::Indent();

					for (EditorNodeProp &prop : _nodeProps[nodeType]) {
						ImGui::Text("%s", prop.propName.c_str());
						ImGui::SameLine();

						ImGui::PushID(node->ID());
						if (prop.propType == EditorNodePropType::Int32) {
							ImGui::InputInt(("##" + prop.propName).c_str(), reinterpret_cast<int *>(prop.getFunction(node)));
						} else if (prop.propType == EditorNodePropType::Float) {
							ImGui::DragFloat(("##" + prop.propName).c_str(), reinterpret_cast<float *>(prop.getFunction(node)));
						} else if (prop.propType == EditorNodePropType::Vec2) {
							ImGui::DragFloat2(("##" + prop.propName).c_str(), &((reinterpret_cast<glm::vec2 *>(prop.getFunction(node)))->x));
						} else if (prop.propType == EditorNodePropType::RID) {
							ImGui::InputInt(("##" + prop.propName).c_str(), reinterpret_cast<RID *>(prop.getFunction(node)));
						} else if (prop.propType == EditorNodePropType::Color) {
							ImGui::ColorEdit3(("##" + prop.propName).c_str(), &reinterpret_cast<Color *>(prop.getFunction(node))->r);
						} else if (prop.propType == EditorNodePropType::String) {
							ImGui::InputText(("##" + prop.propName).c_str(), reinterpret_cast<std::string *>(prop.getFunction(node)));
						} else if (prop.propType == EditorNodePropType::Bool) {
							ImGui::Checkbox(("##" + prop.propName).c_str(), reinterpret_cast<bool *>(prop.getFunction(node)));
						}
						ImGui::PopID();
					}

					ImGui::Unindent();
				}

				const NodeType &typeData = App().GetNodeDB().GetNodeType(nodeType);
				if (typeData.extends != 0) {
					drawNodeProps(node, typeData.extends);
				}
			};

			drawNodeProps(node, node->TypeID());
		}
	}

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();

	if (ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();

		Camera2D *camera = dynamic_cast<Camera2D *>(App().GetCurrentScene()->GetNode(App().GetCurrentScene()->GetCurrentCamera2D()));
		if (camera) {
			ImGui::Text("2D Camera: \"%s\"", camera->Name().c_str());

			ImGui::SameLine();
			if (ImGui::Button("Clear")) {
				App().GetCurrentScene()->SetCurrentCamera2D(0);
			}
		} else
			ImGui::Text("2D Camera: <NONE>");

		ImGui::Unindent();
	}

	ImGui::End();

	ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoBackground);
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
			ImVec2(startPos.x - 4, startPos.y - 4),
			ImVec2(startPos.x + size.x + 4, startPos.y + size.y + 4),
			ImColor(10, 20, 200), 0.f, 0, 4.f);
	}

	if (!App().IsRunning()) {
		if (ImGui::IsItemHovered()) {
			_cameraZoom -= (Input::GetMouseScrollY() * 0.1);
			_cameraZoom = std::clamp(_cameraZoom, 0.1f, 10.f);
			_camera2d.Scale() = glm::vec2(_cameraZoom, _cameraZoom);
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