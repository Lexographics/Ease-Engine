#ifndef MATH_HPP
#define MATH_HPP
#pragma once

#include <cmath>

#include "matrix.hpp"
#include "rect.hpp"
#include "vector2.hpp"

namespace Math {
template <typename T>
inline T Clamp(T value, T min, T max) {
	if (min > max) {
		std::swap(min, max);
	}
	value = std::max(value, min);
	value = std::min(value, max);
	return value;
}

template <typename T>
inline T Lerp(T start, T end, float t) {
	return start + ((end - start) * t);
}

template <>
inline Vector2 Lerp(Vector2 start, Vector2 end, float t) {
	return Vector2(Lerp(start.x, end.x, t), Lerp(start.y, end.y, t));
}

inline float Atan2(float y, float x) {
	return std::atan2(y, x);
}
} // namespace Math

#endif // MATH_HPP