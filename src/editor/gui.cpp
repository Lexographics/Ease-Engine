#ifdef SW_EDITOR
#include "gui.hpp"

#include "imgui.h"

#include "core/application.hpp"
#include "resource/image_texture.hpp"
#include "resource/sprite_sheet_animation.hpp"

bool Gui::TexturePicker(const char *id, RID &rid) {
	bool changed = false;

	ImGui::PushID(id);

	ImTextureID textureID = 0;

	ImageTexture *res = dynamic_cast<ImageTexture *>(App().GetResourceRegistry().GetResource(rid));
	if (res) {
		textureID = (void *)(intptr_t)(res->ID());
	}

	if (ImGui::ImageButton("##Image", textureID, ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0))) {
		ImGui::OpenPopup("##TexturePicker");
	}

	if (ImGui::BeginPopup("##TexturePicker" /*, ImGuiWindowFlags_NoMove */)) {
		ImGui::BeginTable("##Table", 6);

		for (auto &[id, res] : App().GetResourceRegistry().GetResources()) {
			ImageTexture *texture = dynamic_cast<ImageTexture *>(res);
			if (!texture)
				continue;

			ImGui::TableNextColumn();

			if (ImGui::ImageButton(std::to_string(id).c_str(), (void *)(intptr_t)texture->ID(), ImVec2(128, 128), ImVec2(0, 1), ImVec2(1, 0))) {
				rid = id;
				changed = true;
				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndTable();
		ImGui::EndPopup();
	}

	ImGui::PopID();
	return changed;
}

void Gui::AnimationInput(const char *id, RID &rid) {
	ImGui::PushID(id);

	ImGui::InputInt("##RID", &rid);
	if (rid == 0) {
		if (ImGui::Button("Create")) {
			Resource *res = App().GetCurrentScene()->GetResourceLocker().CreateResource("SpriteSheetAnimation");
			rid = res->GetRID();
		}
	} else {
		bool endDisabled = false;
		if (dynamic_cast<SpriteSheetAnimation *>(App().GetResourceRegistry().GetResource(rid)) == nullptr) {
			ImGui::Text("Invalid Animation");
			endDisabled = true;
			ImGui::BeginDisabled();
		}
		if (ImGui::Button("Edit")) {
			App().GetEditor().SetCurrentAnimation(rid);
		}
		if (endDisabled) {
			ImGui::EndDisabled();
		}

		if (ImGui::Button("Delete")) {
			App().GetResourceRegistry().RemoveResourceByID(rid);
			rid = 0;
		}
	}

	ImGui::PopID();
}

bool Gui::BeginFooter(const char *label) {
	ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - ImGui::GetFrameHeight()));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, ImGui::GetFrameHeight()));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
	ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.17f, 0.50f, 0.70f, 1.f));	 // (45, 128, 178)
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.20f, 0.55f, 0.75f, 1.f)); // (51, 140, 191)

	bool opened = ImGui::Begin(label, nullptr,
							   ImGuiWindowFlags_NoCollapse |
								   ImGuiWindowFlags_NoDecoration |
								   ImGuiWindowFlags_NoDocking |
								   ImGuiWindowFlags_NoMove |
								   ImGuiWindowFlags_NoResize |
								   ImGuiWindowFlags_NoSavedSettings |
								   ImGuiWindowFlags_MenuBar |
								   ImGuiWindowFlags_NoScrollbar |
								   ImGuiWindowFlags_NoScrollWithMouse);

	ImGui::PopStyleVar();
	if (!ImGui::BeginMenuBar())
		return false;
	return opened;
}

void Gui::EndFooter() {
	ImGui::EndMenuBar();
	ImGui::End();
	ImGui::PopStyleColor(2);
}
#endif // SW_EDITOR