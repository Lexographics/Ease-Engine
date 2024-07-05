#ifndef SPRITE_SHEET_ANIMATION_HPP
#define SPRITE_SHEET_ANIMATION_HPP
#pragma once

#include "core/debug.hpp"

class SpriteSheet {
  public:
	RID texture = 0;
	glm::ivec2 gridSize = {1, 1};
	std::vector<glm::ivec2> frames;
	float speed = 1.f;

  private:
};

class SpriteSheetAnimation : public Resource {
  public:
	SpriteSheetAnimation() {
		_resourceType = typeid(SpriteSheetAnimation).hash_code();
	}

	SpriteSheet *GetAnimation(const std::string &name) {
		if (!HasAnimation(name))
			return nullptr;
		return &_animations[name];
	}

	const std::unordered_map<std::string, SpriteSheet> &GetAnimations() {
		return _animations;
	}

	void SetAnimation(const std::string &name, const SpriteSheet &anim) {
		_animations[name] = anim;
	}

	bool HasAnimation(const std::string &name) {
		return _animations.find(name) != _animations.end();
	}

	void LoadResource(const Document &doc) override {
		_animations = doc.Get("Animations", _animations);
	}

	void SaveResource(Document &doc) override {
		doc.Set("Animations", _animations);
	}

  private:
	std::unordered_map<std::string, SpriteSheet> _animations;
};

namespace YAML {
template <>
struct convert<SpriteSheet> {
	static Node encode(const SpriteSheet &rhs) {
		Node node;
		node["Texture"] = rhs.texture;
		node["GridSize"] = rhs.gridSize;
		node["Speed"] = rhs.speed;
		node["Frames"] = rhs.frames;
		return node;
	}

	static bool decode(const Node &node, SpriteSheet &rhs) {
		if (!node.IsMap()) {
			return false;
		}

		rhs.texture = node["Texture"].as<RID>(rhs.texture);
		rhs.gridSize = node["GridSize"].as<glm::ivec2>(rhs.gridSize);
		rhs.speed = node["Speed"].as<f32>(rhs.speed);
		rhs.frames = node["Frames"].as<std::vector<glm::ivec2>>(rhs.frames);
		return true;
	}
};
} // namespace YAML

#endif // SPRITE_SHEET_ANIMATION_HPP