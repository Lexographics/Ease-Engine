#include "text2d.hpp"

#include "math/matrix.hpp"

#include "core/application.hpp"
#include "resource/font.hpp"
#include "visual/renderer.hpp"

void Text2D::Update() {
	Font *res = dynamic_cast<Font *>(App().GetResourceRegistry().GetResource(_font));
	if (!res) {
		res = App().GetDefaultFont();
	}

	App().GetRenderer().GetRenderer2D("Text").DrawText(_text, *res, GetTransform(), GetZIndex(), Modulate());
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