#ifndef MATH_HPP
#define MATH_HPP
#pragma once

#include <algorithm>
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

template <typename T>
inline T Map(T v, T input_min, T input_max, T output_min, T output_max) {
	return (v - input_min) * (output_max - output_min) / (input_max - input_min) + output_min;
}

// shortcut for Math::Map with input range 0..1
template <typename T>
inline T TweenMap(T v, T output_min, T output_max) {
	return Map(v, static_cast<T>(0.0), static_cast<T>(1.0), output_min, output_max);
}
} // namespace Math

#endif // MATH_HPP