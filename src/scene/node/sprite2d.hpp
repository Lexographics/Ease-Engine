#ifndef SPRITE2D_HPP
#define SPRITE2D_HPP
#pragma once

#include "sowa.hpp"

#include "data/color.hpp"
#include "node2d.hpp"

class Sprite2D : public Node2D {
  public:
	virtual ~Sprite2D() = default;

	void Update() override;
	bool Serialize(Document &doc) override;
	bool Deserialize(const Document &doc) override;

	bool Copy(Node *dst) override;

	inline RID &GetTexture() { return _texture; }
	inline Color &Modulate() { return _modulate; }

  public:
	RID _texture;
	Color _modulate;
};

#endif // SPRITE2D_HPP