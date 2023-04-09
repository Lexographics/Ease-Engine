#ifndef _E_MATH_HPP__
#define _E_MATH_HPP__
#pragma once

#include "stlpch.hpp"
#include <glm/glm.hpp>
#include "serialize/serializer.hpp"
#include "object_type.hpp"

namespace Sowa {

struct Size : public ObjectType {
	int w;
	int h;

	static std::string Typename() { return "Sowa::Size"; }

	Size() : w(0), h(0) { m_Type = Typename(); }
	Size(int w, int h) : w(w), h(h) { m_Type = Typename(); }
	Size &operator=(std::initializer_list<int> rhs) {
		m_Type = Typename();
		if (rhs.size() != 2) {
			assert(false && "Sowa::Size initializer list must have 2 int elements");
			return *this;
		}

		auto it = rhs.begin();
		w = *it;
		++it;
		h = *it;

		return *this;
	}

	static FileBuffer SaveImpl(ObjectType *);
	static bool LoadImpl(ObjectType* out, const FileBuffer& doc);
};

template <typename T>
struct Vec2 {
	T x;
	T y;

	Vec2(T _x, T _y)
		: x(_x), y(_y) {
	}

	Vec2()
		: x(0.f), y(0.f) {
	}

	Vec2 &operator=(const Vec2 &rhs) {
		x = rhs.x;
		y = rhs.y;

		return *this;
	}

	Vec2 &operator=(const glm::vec2 &rhs) {
		x = rhs.x;
		y = rhs.y;

		return *this;
	}

	operator glm::vec2() {
		return glm::vec2(x, y);
	}

	Vec2 &operator=(std::initializer_list<T> rhs) {
		if (rhs.size() != 2) {
			assert(false && "Sowa::Vec2 initializer list must have 2 float elements");
			return *this;
		}

		auto it = rhs.begin();
		x = *it;
		++it;
		y = *it;

		return *this;
	}

	/// @brief Returns magnitude of vector
	float Length() {
		return sqrt(x * x + y * y);
	}

	/// @brief returns a vector with same direction with magnitude @param length
	Vec2<T> Clamp(float length = 1.0f) {
		float vecLen = Length();
		return Vec2<T>((x / vecLen) * length, (y / vecLen) * length);
	}

	/// @brief returns angle of vector in radians starting from right and rotates clockwise. (1, 0) -> 0 and (0, 1) -> PI / 2
	float Angle() {
		return atan2(y, x);
	}
};

typedef Vec2<float> Vector2;
typedef Vec2<double> Vector2d;

} // namespace Sowa

#endif // _E_MATH_HPP__