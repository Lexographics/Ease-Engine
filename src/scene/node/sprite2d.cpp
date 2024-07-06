#include "sprite2d.hpp"

#include "imgui.h"

#include "core/application.hpp"
#include "core/debug.hpp"
#include "math/matrix.hpp"
#include "resource/image_texture.hpp"
#include "visual/renderer.hpp"

bool Sprite2D::Serialize(Document &doc) {
	if (!Node2D::Serialize(doc))
		return false;

	doc.SetInt("Texture", GetTexture());
	doc.SetColor("Modulate", Modulate());

	return true;
}

bool Sprite2D::Deserialize(const Document &doc) {
	if (!Node2D::Deserialize(doc))
		return false;

	GetTexture() = doc.GetInt("Texture", GetTexture());
	Modulate() = doc.GetColor("Modulate", Modulate());

	return true;
}

bool Sprite2D::Copy(Node *dst) {
	if (!Node2D::Copy(dst)) {
		return false;
	}

	Sprite2D *dstNode = dynamic_cast<Sprite2D *>(dst);
	dstNode->GetTexture() = GetTexture();
	dstNode->Modulate() = Modulate();

	return true;
}

static bool TexturePicker(const char *id, RID &rid) {
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

			if (ImGui::ImageButton(std::to_string(id).c_str(), (void *)(intptr_t)texture->ID(), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0))) {
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

void Sprite2D::UpdateEditor() {
	if (ImGui::CollapsingHeader("Sprite2D", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();

		ImGui::Text("%s", "Texture");
		ImGui::SameLine();
		TexturePicker("##Texture", _texture);

		ImGui::Text("%s", "Modulate");
		ImGui::SameLine();
		ImGui::ColorEdit4("##Modulate", &_modulate.r);

		ImGui::Unindent();
	}
	Node2D::UpdateEditor();
}

void Sprite2D::Update() {
	ImageTexture *res = dynamic_cast<ImageTexture *>(App().GetResourceRegistry().GetResource(_texture));
	if (!res)
		return;

	App().GetRenderer().GetRenderer2D("2D").PushQuad(GetTransform(), res->ID(), glm::vec2(res->Width(), res->Height()), GetZIndex(), Modulate(), ID());
}