#ifndef RECT_HPP
#define RECT_HPP
#pragma once

class Rect {
  public:
	float x = 0.f;
	float y = 0.f;
	float w = 0.f;
	float h = 0.f;

	inline float Left() { return x; }
	inline float Right() { return x + w; }
	inline float Bottom() { return y; }
	inline float Top() { return y + h; }

	Rect() = default;
	Rect(float x, float y) : x(x), y(y), w(0.f), h(0.f) {}
	Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
	~Rect() = default;
};

#endif // RECT_HPP