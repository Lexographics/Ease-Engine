#ifndef CAMERA2D_HPP
#define CAMERA2D_HPP
#pragma once

#include "sowa.hpp"

#include "glm/glm.hpp"
#include "node2d.hpp"

#include "math/rect.hpp"

class Camera2D : public Node2D {
	EDITOR_NODE()
  public:
	virtual ~Camera2D() = default;

	bool Serialize(Document &doc) override;
	bool Deserialize(const Document &doc) override;

	bool Copy(Node *dst) override;

	glm::mat4 GetMatrix();
	static glm::mat4 GetBlankMatrix();

	Rect GetBounds();

	void MakeCurrent();

	inline bool &Rotatable() { return _rotatable; }

  public:
	bool _rotatable = false;
	Vector2 _offset = Vector2(0.f);
	Vector2 _centerPoint = Vector2(0.5f);
};

#endif // CAMERA2D_HPP