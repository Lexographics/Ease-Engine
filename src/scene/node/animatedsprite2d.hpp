#ifndef ANIMATEDSPRITE2D_HPP
#define ANIMATEDSPRITE2D_HPP
#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "node2d.hpp"
#include "resource/image_texture.hpp"
#include "resource/sprite_sheet_animation.hpp"

class AnimatedSprite2D : public Node2D {
  public:
	virtual ~AnimatedSprite2D() = default;

	void Update() override;

	bool Serialize(Document &doc) override;
	bool Deserialize(const Document &doc) override;

	bool Copy(Node *dst) override;
	void UpdateEditor() override;

	// SpriteSheetAnimation
	RID _animation;

	const std::string &GetCurrentAnimation();
	void SetCurrentAnimation(const std::string &name);

  public:
	std::string _currentAnimation = "";
	int _frameIndex = 0;
	float _animationDelta = 0.f;
	float _animationScale = 1.f;
};

#endif // ANIMATEDSPRITE2D_HPP