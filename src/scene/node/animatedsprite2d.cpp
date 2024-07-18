#include "animatedsprite2d.hpp"

#include "imgui.h"

#include "core/application.hpp"
#include "editor/gui.hpp"

void AnimatedSprite2D::Start() {
	_playing = true;
	RestartAnimation();
}

void AnimatedSprite2D::Update() {
	if (!IsVisible())
		return;

	SpriteSheetAnimation *res = dynamic_cast<SpriteSheetAnimation *>(App().GetResourceRegistry().GetResource(_animation));
	if (!res)
		return;

	SpriteSheet *anim = res->GetAnimation(_currentAnimation);
	if (!anim)
		return;

	if (anim->frames.size() == 0)
		return;

	ImageTexture *texture = dynamic_cast<ImageTexture *>(App().GetResourceRegistry().GetResource(anim->texture));
	if (!texture)
		return;

	if (_playing) {
		_animationDelta += std::max(App().Delta() * _animationScale, 0.f);
		if (_animationDelta > anim->speed) {
			// _animationDelta -= anim->speed;
			_animationDelta = 0;
			_frameIndex++;
		}

		_frameIndex = _frameIndex % anim->frames.size();
	}

	glm::vec2 frameSize;
	frameSize.x = 1.f / anim->gridSize.x;
	frameSize.y = 1.f / anim->gridSize.y;

	glm::vec2 topLeft(frameSize.x * anim->frames[_frameIndex].x, 1.f - frameSize.y * anim->frames[_frameIndex].y);

	App().GetRenderer().GetRenderer2D("2D").PushQuad(PushQuadArgs{
		.transform = GetTransform(),
		.textureID = static_cast<float>(texture->ID()),
		.z = static_cast<float>(GetZIndex()),
		.drawID = static_cast<uint32_t>(ID()),
		.color = _modulate,
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
	doc.Set("Playing", _playing);
	doc.SetColor("Modulate", _modulate);
	return true;
}

bool AnimatedSprite2D::Deserialize(const Document &doc) {
	if (!Node2D::Deserialize(doc)) {
		return false;
	}

	_animation = doc.Get("Animation", _animation);
	_currentAnimation = doc.Get("CurrentAnimation", _currentAnimation);
	_animationScale = doc.Get("AnimationScale", _animationScale);
	_playing = doc.Get("Playing", _playing);
	_modulate = doc.GetColor("Modulate", _modulate);
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
	dstNode->_playing = _playing;
	dstNode->_modulate = _modulate;
	dstNode->_frameIndex = _frameIndex;
	dstNode->_animationDelta = _animationDelta;
	return true;
}

void AnimatedSprite2D::UpdateEditor() {
	if (ImGui::CollapsingHeader("AnimatedSprite2D", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();

		ImGui::Text("%s", "Animation");
		ImGui::SameLine();
		Gui::AnimationInput("##Animation", _animation);

		SpriteSheetAnimation *animation = dynamic_cast<SpriteSheetAnimation *>(App().GetResourceRegistry().GetResource(_animation));

		ImGui::Text("%s", "Current Animation");
		ImGui::SameLine();
		if (ImGui::BeginCombo("##CurrentAnimation", _currentAnimation.c_str())) {
			if (animation) {
				for (auto &[name, anim] : animation->GetAnimations()) {
					if (ImGui::Selectable(name.c_str())) {
						SetCurrentAnimation(name);
					}
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Text("%s", "Animation Scale");
		ImGui::SameLine();
		ImGui::DragFloat("##AnimationScale", &_animationScale, 0.1f, 0.f);

		ImGui::Text("%s", "Playing");
		ImGui::SameLine();
		if (ImGui::Checkbox("##Playing", &_playing)) {
			if (_playing) {
				RestartAnimation();
			}
		}

		ImGui::Text("%s", "Modulate");
		ImGui::SameLine();
		ImGui::ColorEdit4("##Modulate", &_modulate.r);

		ImGui::Unindent();
	}
	Node2D::UpdateEditor();
}

const std::string &AnimatedSprite2D::GetCurrentAnimation() {
	return _currentAnimation;
}

void AnimatedSprite2D::SetCurrentAnimation(const std::string &name, bool reset /* = true*/) {
	_currentAnimation = name;
	if (reset)
		RestartAnimation();
}

void AnimatedSprite2D::RestartAnimation() {
	_frameIndex = 0;
	_animationDelta = 0.f;
}