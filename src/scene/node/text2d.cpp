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