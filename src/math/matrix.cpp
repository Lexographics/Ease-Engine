#include "matrix.hpp"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Matrix::CalculateTransform(glm::vec2 position, float rotation, glm::vec2 scale, const glm::mat4 &base /*= glm::mat4(1.f)*/) {
	glm::mat4 transform = glm::translate(base, {position.x, position.y, 0.f});
	transform = glm::rotate(transform, glm::radians(-rotation), {0.f, 0.f, 1.f});
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