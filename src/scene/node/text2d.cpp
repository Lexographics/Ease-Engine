#include "text2d.hpp"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

#include "core/application.hpp"
#include "math/matrix.hpp"
#include "resource/font.hpp"
#include "visual/renderer.hpp"

void Text2D::Update() {
	if (!IsVisible())
		return;

	Font *res = dynamic_cast<Font *>(App().GetResourceRegistry().GetResource(_font));
	if (!res) {
		res = App().GetDefaultFont();
	}

	App().GetRenderer().GetRenderer2D("Text").DrawText(_text, *res, GetTransform(), GetZIndex(), Modulate(), ID());
}

bool Text2D::Serialize(Document &doc) {
	if (!Node2D::Serialize(doc))
		return false;

	doc.SetString("Text", Text());
	doc.SetInt("Font", GetFont());
	doc.SetColor("Modulate", Modulate());

	return true;
}

bool Text2D::Deserialize(const Document &doc) {
	if (!Node2D::Deserialize(doc))
		return false;

	Text() = doc.GetString("Text", Text());
	GetFont() = doc.GetInt("Font", GetFont());
	Modulate() = doc.GetColor("Modulate", Modulate());

	return true;
}

bool Text2D::Copy(Node *dst) {
	if (!Node2D::Copy(dst)) {
		return false;
	}

	Text2D *dstNode = dynamic_cast<Text2D *>(dst);
	dstNode->Text() = Text();
	dstNode->GetFont() = GetFont();
	dstNode->Modulate() = Modulate();

	return true;
}

#ifdef SW_EDITOR
void Text2D::UpdateEditor() {
	if (ImGui::CollapsingHeader("Text2D", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();

		ImGui::Text("%s", "Text");
		ImGui::SameLine();
		ImGui::InputText("##Text", &_text);

		ImGui::Text("%s", "Modulate");
		ImGui::SameLine();
		ImGui::ColorEdit4("##Modulate", &_modulate.r);

		ImGui::Text("%s", "Font");
		ImGui::SameLine();
		int font = static_cast<int>(_font);
		ImGui::InputInt("##Font", &font);
		_font = static_cast<RID>(font);

		ImGui::Unindent();
	}

	Node2D::UpdateEditor();
}
#endif