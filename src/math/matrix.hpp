#ifndef MATRIX_HPP
#define MATRIX_HPP
#pragma once

#include <glm/glm.hpp>

#include "math/vector2.hpp"

namespace Matrix {
glm::mat4 CalculateTransform(Vector2 position, float rotation, Vector2 scale, Vector2 offset = Vector2(0, 0), const glm::mat4 &base = glm::mat4(1.f));
glm::mat4 CalculateTransform3D(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, const glm::mat4 &base = glm::mat4(1.f));

bool DecomposeTransform(const glm::mat4 &mat, Vector2 *position, float *rotation, Vector2 *scale);

glm::mat4 CalculateOrtho(float left, float right, float bottom, float top);
} // namespace Matrix

#endif // MATRIX_HPP