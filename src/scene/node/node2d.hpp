#ifndef NODE2D_HPP
#define NODE2D_HPP
#pragma once

#include "glm/glm.hpp"

#include "scene/node.hpp"

class Node2D : public Node {
  public:
	virtual ~Node2D() = default;

	glm::mat4 GetTransform();
	glm::mat4 GetLocalTransform();
	glm::mat4 GetParentTransform();

	inline glm::vec2 &Position() { return _position; }
	inline float &Rotation() { return _rotation; }
	inline glm::vec2 &Scale() { return _scale; }

  private:
	glm::vec2 _position{0.f, 0.f};
	float _rotation{0.f};
	glm::vec2 _scale{1.f, 1.f};
};

#endif // NODE2D_HPP