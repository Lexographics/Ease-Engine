#include "sprite2d.hpp"

#include "math/matrix.hpp"

#include "core/application.hpp"
#include "resource/image_texture.hpp"
#include "visual/renderer.hpp"

void Sprite2D::Update() {
	ImageTexture *res = dynamic_cast<ImageTexture *>(App().GetResourceRegistry().GetResource(_texture));
	if (!res)
		return;

	App().GetRenderer().GetRenderer2D("2D").PushQuad(GetTransform(), res->ID(), glm::vec2(res->Width(), res->Height()), GetZIndex(), Modulate(), ID());
}