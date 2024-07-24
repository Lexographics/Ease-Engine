#include "camera2d.hpp"

#include "imgui.h"

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
	glm::mat4 transform;
	if (!Rotatable()) {
		Vector2 position;
		Vector2 scale;
		Matrix::DecomposeTransform(GetTransform(), &position, nullptr, &scale);

		transform = Matrix::CalculateTransform(position, 0, scale, _offset);
	} else {
		transform = GetTransform(_offset);
	}
	transform = glm::inverse(transform);

	return Matrix::CalculateOrtho(-640.f, 640.f, -360.f, 360.f) * transform;
}

// static
glm::mat4 Camera2D::GetBlankMatrix() {
	return Matrix::CalculateOrtho(-640.f, 640.f, -360.f, 360.f);
}

Rect Camera2D::GetBounds() {
	auto mat = GetTransform();

	Vector2 position;
	float rotation;
	Vector2 scale;
	Matrix::DecomposeTransform(mat, &position, &rotation, &scale);

	Rect rect;
	rect.x = position.x - (640 * scale.x);
	rect.y = position.y - (360 * scale.y);
	rect.w = 1280 * scale.x;
	rect.h = 720 * scale.y;

	return rect;
}

void Camera2D::MakeCurrent() {
	App().GetCurrentScene()->SetCurrentCamera2D(this->ID());
}

EDITOR_UPDATE_FUNC(Camera2D, {
	if (ImGui::CollapsingHeader("Camera2D", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();

		ImGui::Text("%s", "Offset");
		ImGui::SameLine();
		ImGui::DragFloat2("##Offset", &_offset.x);

		ImGui::Text("%s", "Rotatable");
		ImGui::SameLine();
		ImGui::Checkbox("##Rotatable", &_rotatable);

		ImGui::Unindent();
	}

	Node2D::UpdateEditor();
})