#include "camera2d.hpp"

#include "core/application.hpp"
#include "math/matrix.hpp"

bool Camera2D::Serialize(Document &doc) {
	if (!Node2D::Serialize(doc))
		return false;

	return true;
}

bool Camera2D::Deserialize(const Document &doc) {
	if (!Node2D::Deserialize(doc))
		return false;

	return true;
}

bool Camera2D::Copy(Node *dst) {
	if (!Node2D::Copy(dst)) {
		return false;
	}

	Camera2D *dstNode = dynamic_cast<Camera2D *>(dst);
	dstNode->Rotatable() = Rotatable();

	return true;
}

glm::mat4 Camera2D::GetMatrix() {
	glm::mat4 transform = GetTransform();
	if (!Rotatable()) {
		glm::vec2 position;
		glm::vec2 scale;
		Matrix::DecomposeTransform(transform, &position, nullptr, &scale);

		transform = Matrix::CalculateTransform(position, 0, scale);
	}
	transform = glm::inverse(transform);

	return Matrix::CalculateOrtho(-640.f, 640.f, -360.f, 360.f) * transform;
}

// static
glm::mat4 Camera2D::GetBlankMatrix() {
	return Matrix::CalculateOrtho(-640.f, 640.f, -360.f, 360.f);
}

void Camera2D::MakeCurrent() {
	App().GetCurrentScene()->SetCurrentCamera2D(this->ID());
}