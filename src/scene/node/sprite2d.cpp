#include "sprite2d.hpp"

#include "math/matrix.hpp"

#include "core/application.hpp"
#include "core/debug.hpp"
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

void Sprite2D::Update() {
	ImageTexture *res = dynamic_cast<ImageTexture *>(App().GetResourceRegistry().GetResource(_texture));
	if (!res)
		return;

	App().GetRenderer().GetRenderer2D("2D").PushQuad(GetTransform(), res->ID(), glm::vec2(res->Width(), res->Height()), GetZIndex(), Modulate(), ID());
}