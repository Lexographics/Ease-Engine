#ifndef EASING_HPP
#define EASING_HPP
#pragma once

enum class Easing {
	Linear = 0,

	SineEaseIn,
	SineEaseOut,
	SineEaseInOut,

	CubicEaseIn,
	CubicEaseOut,
	CubicEaseInOut,

	QuintIn,
	QuintOut,
	QuintInOut,

	CircIn,
	CircOut,
	CircInOut,

	ElasticIn,
	ElasticOut,
	ElasticInOut,

	QuadIn,
	QuadOut,
	QuadInOut,

	QuartIn,
	QuartOut,
	QuartInOut,

	ExpoIn,
	ExpoOut,
	ExpoInOut,

	BackIn,
	BackOut,
	BackInOut,

	BounceIn,
	BounceOut,
	BounceInOut,

	EasingCount,
};

namespace Utils {

// maps t in range(0, 1) to given easing function
float LerpMap(float t, Easing easing = Easing::Linear);
float Lerp(float from, float to, float t, Easing easing = Easing::Linear);

} // namespace Utils

#endif // EASING_HPP