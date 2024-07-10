#ifndef MATH_HPP
#define MATH_HPP
#pragma once

#include <cmath>

#include "matrix.hpp"
#include "rect.hpp"
#include "vector2.hpp"

namespace Math {
template <typename T>
T Clamp(T value, T min, T max) {
	if (min > max) {
		std::swap(min, max);
	}
	value = std::max(value, min);
	value = std::min(value, max);
	return value;
}
} // namespace Math

#endif // MATH_HPP