#include "camera2d.hpp"

#include "imgui.h"

#include "core/application.hpp"
#include "math/matrix.hpp"

bool Camera2D::Serialize(Document &doc) {
	if (!Node2D::Serialize(doc))
		return false;

	doc.Set("Rotatable", _rotatable);
	doc.SetVec2("Offset", _offset);
	doc.SetVec2("CenterPoint", _centerPoint);

	return true;
}

bool Camera2D::Deserialize(const Document &doc) {
	if (!Node2D::Deserialize(doc))
		return false;

	_rotatable = doc.Get("Rotatable", _rotatable);
	_offset = doc.GetVec2("Offset", _offset);
	_centerPoint = doc.GetVec2("CenterPoint", _centerPoint);

	return true;
}

bool Camera2D::Copy(Node *dst) {
	if (!Node2D::Copy(dst)) {
		return false;
	}

	Camera2D *dstNode = dynamic_cast<Camera2D *>(dst);
	dstNode->Rotatable() = Rotatable();
	dstNode->_offset = _offset;
	dstNode->_centerPoint = _centerPoint;

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

	Vector2 videoSize = Vector2(
		App().GetProjectSettings().rendering.viewport.width,
		App().GetProjectSettings().rendering.viewport.height);

	float left = (-videoSize.x) + (videoSize.x * _centerPoint.x);
	float bottom = (-videoSize.y) + (videoSize.y * _centerPoint.y);

	return Matrix::CalculateOrtho(left, left + videoSize.x, bottom, bottom + videoSize.y) * transform;
}

// static
glm::mat4 Camera2D::GetBlankMatrix() {
	Vector2 videoSize = Vector2(
		App().GetProjectSettings().rendering.viewport.width,
		App().GetProjectSettings().rendering.viewport.height);

	return Matrix::CalculateOrtho(0.f, videoSize.x, 0.f, videoSize.y);
}

Rect Camera2D::GetBounds() {
	auto mat = GetTransform();

	Vector2 position;
	float rotation;
	Vector2 scale;
	Matrix::DecomposeTransform(mat, &position, &rotation, &scale);

	Vector2 videoSize = Vector2(
		App().GetProjectSettings().rendering.viewport.width,
		App().GetProjectSettings().rendering.viewport.height);

	Rect rect;
	rect.x = position.x + (-videoSize.x + (videoSize.x * _centerPoint.x * scale.x));
	rect.y = position.y + (-videoSize.y + (videoSize.y * _centerPoint.y * scale.y));
	rect.w = videoSize.x * scale.x;
	rect.h = videoSize.y * scale.y;

	return rect;
}

void Camera2D::MakeCurrent() {
	App().GetCurrentScene()->SetCurrentCamera2D(this->ID());
}

#ifdef SW_EDITOR
void Camera2D::UpdateEditor() {
	if (ImGui::CollapsingHeader("Camera2D", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();

		ImGui::Text("%s", "Offset");
		ImGui::SameLine();
		ImGui::DragFloat2("##Offset", &_offset.x);

		ImGui::Text("%s", "Rotatable");
		ImGui::SameLine();
		ImGui::Checkbox("##Rotatable", &_rotatable);

		ImGui::Text("%s", "Center Point");
		ImGui::SameLine();
		ImGui::DragFloat2("##Center Point", &_centerPoint.x, 0.01f, 0.f, 1.f);

		ImGui::Unindent();
	}

	Node2D::UpdateEditor();
}
#endif