#include "matrix.hpp"

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Matrix::CalculateTransform(glm::vec2 position, float rotation, glm::vec2 scale, const glm::mat4 &base /*= glm::mat4(1.f)*/) {
	glm::mat4 transform = glm::translate(base, {position.x, position.y, 0.f});
	transform = glm::rotate(transform, glm::radians(-rotation), {0.f, 0.f, 1.f});
	transform = glm::scale(transform, {scale.x, scale.y, 1.f});

	return transform;
}