#include "progress_bar.hpp"

#include "imgui.h"

#include "glm/gtc/matrix_transform.hpp"

#include "core/application.hpp"
#include "core/debug.hpp"
#include "editor/gui.hpp"
#include "math/matrix.hpp"
#include "resource/image_texture.hpp"
#include "visual/renderer.hpp"

bool ProgressBar::Serialize(Document &doc) {
	if (!Node2D::Serialize(doc))
		return false;

	doc.Set("MinValue", _minValue);
	doc.Set("MaxValue", _maxValue);
	doc.Set("Value", _value);
	doc.SetVec2("Size", _size);
	doc.Set("Padding", _padding);
	doc.SetColor("ForegroundColor", _foregroundColor);
	doc.SetColor("BackgroundColor", _backgroundColor);

	return true;
}

bool ProgressBar::Deserialize(const Document &doc) {
	if (!Node2D::Deserialize(doc))
		return false;

	_minValue = doc.Get("MinValue", _minValue);
	_maxValue = doc.Get("MaxValue", _maxValue);
	_value = doc.Get("Value", _value);
	_size = doc.GetVec2("Size", _size);
	_padding = doc.Get("Padding", _padding);
	_foregroundColor = doc.GetColor("ForegroundColor", _foregroundColor);
	_backgroundColor = doc.GetColor("BackgroundColor", _backgroundColor);

	return true;
}

bool ProgressBar::Copy(Node *dst) {
	if (!Node2D::Copy(dst)) {
		return false;
	}

	ProgressBar *dstNode = dynamic_cast<ProgressBar *>(dst);
	dstNode->_minValue = _minValue;
	dstNode->_maxValue = _maxValue;
	dstNode->_value = _value;
	dstNode->_size = _size;
	dstNode->_padding = _padding;
	dstNode->_foregroundColor = _foregroundColor;
	dstNode->_backgroundColor = _backgroundColor;

	return true;
}

void ProgressBar::Update() {
	if (!IsVisible())
		return;

	float value = (_value - _minValue) / (_maxValue - _minValue);
	float yWidth = _size.x * value;

	glm::mat4 transform = GetTransform();
	Vector2 position;
	float rotation;
	Matrix::DecomposeTransform(transform, &position, &rotation, nullptr);

	auto mat = Matrix::CalculateTransform(position, -rotation, Vector2(1.f, 1.f), Vector2(0.f, 0.f));
	auto innerMat = glm::translate(mat, glm::vec3((_size.x * 0.5f * value) - _size.x * 0.5f, 0.f, 0.f));

	App().GetRenderer().GetRenderer2D("2D").PushQuad(innerMat, 0.f, glm::vec2(yWidth - _padding * 2, _size.y - _padding * 2), GetZIndex(), _foregroundColor, ID());
	App().GetRenderer().GetRenderer2D("2D").PushQuad(mat, 0.f, glm::vec2(_size.x, _size.y), GetZIndex(), _backgroundColor, ID());
}

EDITOR_UPDATE_FUNC(ProgressBar, {
	if (ImGui::CollapsingHeader("Progress Bar", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();

		ImGui::Text("%s", "Range");
		ImGui::SameLine();
		ImGui::DragFloatRange2("##Range", &_minValue, &_maxValue);

		ImGui::Text("%s", "Value");
		ImGui::SameLine();
		ImGui::DragFloat("##Value", &_value);

		ImGui::Text("%s", "Size");
		ImGui::SameLine();
		ImGui::DragFloat2("##Size", &_size.x);

		ImGui::Text("%s", "Padding");
		ImGui::SameLine();
		ImGui::DragFloat("##Padding", &_padding, 0.1f, 0.f, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp);

		ImGui::Text("%s", "Foreground");
		ImGui::SameLine();
		ImGui::ColorEdit4("##Foreground", &_foregroundColor.r);

		ImGui::Text("%s", "Background");
		ImGui::SameLine();
		ImGui::ColorEdit4("##Background", &_backgroundColor.r);

		ImGui::Unindent();
	}
	Node2D::UpdateEditor();
})