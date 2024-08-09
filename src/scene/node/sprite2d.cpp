#include "sprite2d.hpp"

#include "imgui.h"

#include "core/application.hpp"
#include "core/debug.hpp"
#include "editor/gui.hpp"
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

void Sprite2D::Update() {
	if (!IsVisible())
		return;

	ImageTexture *res = dynamic_cast<ImageTexture *>(App().GetResourceRegistry().GetResource(_texture));
	if (!res)
		return;

	App().GetRenderer().GetRenderer2D("2D").PushQuad(GetTransform(), res->ID(), glm::vec2(res->Width(), res->Height()), GetZIndex(), Modulate(), ID());
}

#ifdef SW_EDITOR
void Sprite2D::UpdateEditor() {
	if (ImGui::CollapsingHeader("Sprite2D", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();

		ImGui::Text("%s", "Texture");
		ImGui::SameLine();
		Gui::TexturePicker("##Texture", _texture);

		ImGui::Text("%s", "Modulate");
		ImGui::SameLine();
		ImGui::ColorEdit4("##Modulate", &_modulate.r);

		ImGui::Unindent();
	}
	Node2D::UpdateEditor();
}
#endif