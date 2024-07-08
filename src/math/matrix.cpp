#include "matrix.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"

glm::mat4 Matrix::CalculateTransform(glm::vec2 position, float rotation, glm::vec2 scale, glm::vec2 offset /* = glm::vec2(0, 0)*/, const glm::mat4 &base /*= glm::mat4(1.f)*/) {
	glm::mat4 transform = glm::translate(base, {position.x, position.y, 0.f});
	transform = glm::rotate(transform, glm::radians(-rotation), {0.f, 0.f, 1.f});
	transform = glm::translate(transform, {offset.x, offset.y, 0});
	transform = glm::scale(transform, {scale.x, scale.y, 1.f});

	return transform;
}

glm::mat4 Matrix::CalculateTransform3D(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, const glm::mat4 &base /*= glm::mat4(1.f)*/) {
	glm::mat4 transform = glm::translate(base, {position.x, position.y, position.z});
	transform = glm::rotate(transform, glm::radians(rotation.z), {0.f, 0.f, 1.f});
	transform = glm::rotate(transform, glm::radians(rotation.y), {0.f, 1.f, 0.f});
	transform = glm::rotate(transform, glm::radians(rotation.x), {1.f, 0.f, 0.f});
	transform = glm::scale(transform, {scale.x, scale.y, scale.z});

	return transform;
}

bool Matrix::DecomposeTransform(const glm::mat4 &mat, glm::vec2 *position, float *rotation, glm::vec2 *scale) {
	glm::vec3 _position;
	glm::vec3 _scale;
	glm::quat _orientation;
	glm::vec3 _skew;
	glm::vec4 _perspective;

	bool ok = glm::decompose(mat, _scale, _orientation, _position, _skew, _perspective);
	if (!ok)
		return false;

	if (position)
		*position = glm::vec2(_position.x, _position.y);
	if (rotation)
		*rotation = glm::degrees(glm::eulerAngles(_orientation).z);
	if (scale)
		*scale = glm::vec2(_scale.x, _scale.y);

	return true;
}

glm::mat4 Matrix::CalculateOrtho(float left, float right, float bottom, float top) {
	return glm::ortho(left, right, bottom, top, -128.f, 128.f);
}