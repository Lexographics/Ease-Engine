#include "gui.hpp"

#include "imgui.h"

#include "core/application.hpp"
#include "resource/image_texture.hpp"

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
			Resource *res = App().GetResourceRegistry().CreateResource("SpriteSheetAnimation");
			App().GetResourceRegistry().AddResource(res, 0);
			rid = res->GetRID();
		}
	} else {
		if (ImGui::Button("Edit")) {
			App().GetEditor().SetCurrentAnimation(rid);
		}

		if (ImGui::Button("Delete")) {
			App().GetResourceRegistry().RemoveResourceByID(rid);
			rid = 0;
		}
	}

	ImGui::PopID();
}