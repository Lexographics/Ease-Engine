#include "animatedsprite2d.hpp"

#include "imgui.h"

#include "core/application.hpp"

void AnimatedSprite2D::Update() {
	SpriteSheetAnimation *res = dynamic_cast<SpriteSheetAnimation *>(App().GetResourceRegistry().GetResource(_animation));
	if (!res)
		return;

	SpriteSheet *anim = res->GetAnimation(_currentAnimation);
	if (!anim)
		return;

	ImageTexture *texture = dynamic_cast<ImageTexture *>(App().GetResourceRegistry().GetResource(anim->texture));
	if (!texture)
		return;

	float delta = 1.f / 60;

	_animationDelta += std::max(delta * _animationScale, 0.f);
	if (_animationDelta > anim->speed) {
		_animationDelta -= anim->speed;
		_frameIndex++;
	}

	_frameIndex = _frameIndex % anim->frames.size();

	glm::vec2 frameSize;
	frameSize.x = 1.f / anim->gridSize.x;
	frameSize.y = 1.f / anim->gridSize.y;

	glm::vec2 topLeft(frameSize.x * anim->frames[_frameIndex].x, 1.f - frameSize.y * anim->frames[_frameIndex].y);

	App().GetRenderer().GetRenderer2D("2D").PushQuad(PushQuadArgs{
		.transform = GetTransform(),
		.textureID = static_cast<float>(texture->ID()),
		.z = static_cast<float>(GetZIndex()),
		.drawID = static_cast<float>(ID()),
		.color = Color(1.f),
		.textureScale = glm::vec2(texture->Width() * frameSize.x, texture->Height() * frameSize.y),
		.uvTopLeft = topLeft,
		.uvBottomRight = glm::vec2(topLeft.x + frameSize.x, topLeft.y - frameSize.y)});
}

bool AnimatedSprite2D::Serialize(Document &doc) {
	if (!Node2D::Serialize(doc)) {
		return false;
	}

	doc.Set("Animation", _animation);
	doc.Set("CurrentAnimation", _currentAnimation);
	doc.Set("AnimationScale", _animationScale);
	return true;
}

bool AnimatedSprite2D::Deserialize(const Document &doc) {
	if (!Node2D::Deserialize(doc)) {
		return false;
	}

	_animation = doc.Get("Animation", _animation);
	_currentAnimation = doc.Get("CurrentAnimation", _currentAnimation);
	_animationScale = doc.Get("CurrentAnimation", _animationScale);
	return true;
}

bool AnimatedSprite2D::Copy(Node *dst) {
	if (!Node2D::Copy(dst)) {
		return false;
	}

	AnimatedSprite2D *dstNode = dynamic_cast<AnimatedSprite2D *>(dst);
	dstNode->_animation = _animation;
	dstNode->SetCurrentAnimation(_currentAnimation);
	dstNode->_animationScale = _animationScale;
	return true;
}

void AnimatedSprite2D::UpdateEditor() {
	if (ImGui::CollapsingHeader("AnimatedSprite2D", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();

		ImGui::Text("%s", "Animation");
		ImGui::SameLine();
		ImGui::InputInt("##Animation", &_animation);

		ImGui::Text("%s", "Animation Scale");
		ImGui::SameLine();
		ImGui::DragFloat("##AnimationScale", &_animationScale, 0.1f, 0.f);

		ImGui::Unindent();
	}
	Node2D::UpdateEditor();
}

const std::string &AnimatedSprite2D::GetCurrentAnimation() {
	return _currentAnimation;
}

void AnimatedSprite2D::SetCurrentAnimation(const std::string &name) {
	_currentAnimation = name;
	_frameIndex = 0;
	_animationDelta = 0.f;
}
