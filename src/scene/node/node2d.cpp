#include "node2d.hpp"

#include "math/matrix.hpp"

bool Node2D::Serialize(Document &doc) {
	if (!Node::Serialize(doc))
		return false;

	doc.SetVec2("Position", Position());
	doc.SetFloat("Rotation", Rotation());
	doc.SetVec2("Scale", Scale());

	return true;
}

bool Node2D::Deserialize(const Document &doc) {
	if (!Node::Deserialize(doc))
		return false;

	Position() = doc.GetVec2("Position", Position());
	Rotation() = doc.GetFloat("Rotation", Rotation());
	Scale() = doc.GetVec2("Scale", Scale());

	return true;
}

bool Node2D::Copy(Node *dst) {
	if (!Node::Copy(dst)) {
		return false;
	}

	Node2D *dstNode = dynamic_cast<Node2D *>(dst);
	dstNode->Position() = Position();
	dstNode->Rotation() = Rotation();
	dstNode->Scale() = Scale();
	dstNode->ZIndex() = ZIndex();

	return true;
}

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