#include "rect.hpp"

#include "math/math.hpp"

Vector2 Rect::MapPoint(const Vector2 &point, const Rect &target) const {
	return Vector2(
		Math::Map(point.x, this->x, this->x + this->w, target.x, target.x + target.w),
		Math::Map(point.y, this->y, this->y + this->h, target.y, target.y + target.h));
}