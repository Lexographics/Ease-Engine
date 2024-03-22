#ifndef MATRIX_HPP
#define MATRIX_HPP
#pragma once

#include <glm/glm.hpp>

namespace Matrix {
glm::mat4 CalculateTransform(glm::vec2 position, float rotation, glm::vec2 scale, const glm::mat4 &base = glm::mat4(1.f));
glm::mat4 CalculateTransform3D(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, const glm::mat4 &base = glm::mat4(1.f));
} // namespace Matrix

#endif // MATRIX_HPP