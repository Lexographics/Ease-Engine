#ifndef SPRITE2D_HPP
#define SPRITE2D_HPP
#pragma once

#include "sowa.hpp"

#include "node2d.hpp"

class Sprite2D : public Node2D {
  public:
	virtual ~Sprite2D() = default;

	void Update() override;

	inline RID &GetTexture() { return _texture; }

  private:
	RID _texture;
};

#endif // SPRITE2D_HPP