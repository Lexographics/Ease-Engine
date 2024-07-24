#ifndef ANIMATEDSPRITE2D_HPP
#define ANIMATEDSPRITE2D_HPP
#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "node2d.hpp"
#include "resource/image_texture.hpp"
#include "resource/sprite_sheet_animation.hpp"

class AnimatedSprite2D : public Node2D {
	EDITOR_NODE()
  public:
	virtual ~AnimatedSprite2D() = default;

	void Start() override;
	void Update() override;

	bool Serialize(Document &doc) override;
	bool Deserialize(const Document &doc) override;

	bool Copy(Node *dst) override;

	const std::string &GetCurrentAnimation();
	void SetCurrentAnimation(const std::string &name, bool reset = true);

	void RestartAnimation();

  public:
	// SpriteSheetAnimation
	RID _animation;
	float _animationScale = 1.f;
	bool _playing = false;
	Color _modulate;

  private:
	int _frameIndex = 0;
	std::string _currentAnimation = "";
	float _animationDelta = 0.f;
};

#endif // ANIMATEDSPRITE2D_HPP