#ifndef VECTOR2_HPP
#define VECTOR2_HPP
#pragma once

#include <cmath>

class Vector2 {
  public:
	Vector2() : x(0.f), y(0.f) {}
	Vector2(float v) : x(v), y(v) {}
	Vector2(float x, float y) : x(x), y(y) {}

	float x;
	float y;

	float Length() const {
		return std::sqrt(x * x + y * y);
	}

	float LengthSquared() const {
		return x * x + y * y;
	}

	float Distance(const Vector2 &point) const {
		return Vector2(point - *this).Length();
	}

	Vector2 operator-() const {
		return Vector2(-x, -y);
	}
	Vector2 operator-(const Vector2 &obj) const {
		return Vector2(x - obj.x, y - obj.y);
	}
	Vector2 &operator-=(const Vector2 &obj) {
		x -= obj.x;
		y -= obj.y;
		return *this;
	}

	Vector2 operator+(const Vector2 &obj) const {
		return Vector2(x + obj.x, y + obj.y);
	}
	Vector2 &operator+=(const Vector2 &obj) {
		x += obj.x;
		y += obj.y;
		return *this;
	}

	Vector2 operator*(const Vector2 &obj) const {
		return Vector2(x * obj.x, y * obj.y);
	}
	Vector2 &operator*=(const Vector2 &obj) {
		x *= obj.x;
		y *= obj.y;
		return *this;
	}
	Vector2 operator*(float v) const {
		return Vector2(x * v, y * v);
	}
	Vector2 &operator*=(float v) {
		x *= v;
		y *= v;
		return *this;
	}

	Vector2 operator/(const Vector2 &obj) const {
		return Vector2(x / obj.x, y / obj.y);
	}
	Vector2 &operator/=(const Vector2 &obj) {
		x /= obj.x;
		y /= obj.y;
		return *this;
	}
	Vector2 operator/(float v) const {
		return Vector2(x / v, y / v);
	}
	Vector2 &operator/=(float v) {
		x /= v;
		y /= v;
		return *this;
	}

	bool operator==(const Vector2 &obj) const {
		return x == obj.x && y == obj.y;
	}

  private:
};

#endif // VECTOR2_HPP