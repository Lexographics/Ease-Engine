#include "node2d.hpp"

#include "math/matrix.hpp"

glm::mat4 Node2D::GetTransform() {
	return Matrix::CalculateTransform(_position, _rotation, _scale, GetParentTransform());
}

glm::mat4 Node2D::GetLocalTransform() {
	return Matrix::CalculateTransform(_position, _rotation, _scale);
}

glm::mat4 Node2D::GetParentTransform() {
	if (Node2D *parent = dynamic_cast<Node2D *>(GetParent()); nullptr != parent) {
		return parent->GetTransform();
	}

	return glm::mat4(1.f);
}

int Node2D::GetZIndex() {
	if (Node2D *parent = dynamic_cast<Node2D *>(GetParent()); nullptr != parent) {
		return _zIndex + parent->GetZIndex();
	}

	return _zIndex;
}