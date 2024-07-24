#ifndef NODE2D_HPP
#define NODE2D_HPP
#pragma once

#include "glm/glm.hpp"

#include "math/vector2.hpp"
#include "scene/node.hpp"

class Node2D : public Node {
	EDITOR_NODE()
  public:
	virtual ~Node2D() = default;

	bool Serialize(Document &doc) override;
	bool Deserialize(const Document &doc) override;

	bool Copy(Node *dst) override;

	glm::mat4 GetTransform(const Vector2 &offset = Vector2(0.f, 0.f));
	glm::mat4 GetLocalTransform(const Vector2 &offset = Vector2(0.f, 0.f));
	glm::mat4 GetParentTransform();
	int GetZIndex();
	bool IsVisible();

	Vector2 GetGlobalPosition();

	inline Vector2 &Position() { return _position; }
	inline float &Rotation() { return _rotation; }
	inline Vector2 &Scale() { return _scale; }
	inline int &ZIndex() { return _zIndex; }

  public:
	Vector2 _position{0.f, 0.f};
	float _rotation{0.f};
	Vector2 _scale{1.f, 1.f};
	int _zIndex = 0;
	bool _visible = true;
};

#endif // NODE2D_HPP