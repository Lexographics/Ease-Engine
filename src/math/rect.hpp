#ifndef RECT_HPP
#define RECT_HPP
#pragma once

class Vector2;

class Rect {
  public:
	float x = 0.f;
	float y = 0.f;
	float w = 0.f;
	float h = 0.f;

	inline float Left() const { return x; }
	inline float Right() const { return x + w; }
	inline float Bottom() const { return y; }
	inline float Top() const { return y + h; }

	Rect() = default;
	Rect(float x, float y) : x(x), y(y), w(0.f), h(0.f) {}
	Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
	~Rect() = default;

	Vector2 MapPoint(const Vector2 &point, const Rect &target) const;
};

#endif // RECT_HPP