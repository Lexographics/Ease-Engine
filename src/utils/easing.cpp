#include "easing.hpp"

#include "core/debug.hpp"

#ifndef PI
#define PI (3.14159265358979323846)
#endif

float Utils::LerpMap(float t, Easing easing) {
	if (easing == Easing::Linear) {
		return t;
	}
	if (easing == Easing::SineEaseIn) {
		return 1.f - std::cos((t * PI) * 0.5f);
	}
	if (easing == Easing::SineEaseOut) {
		return std::sin((t * PI) * 0.5f);
	}
	if (easing == Easing::SineEaseInOut) {
		return -(std::cos(PI * t) - 1.f) * 0.5f;
	}
	if (easing == Easing::CubicEaseIn) {
		return t * t * t;
	}
	if (easing == Easing::CubicEaseOut) {
		return 1.f - ((1.f - t) * (1.f - t) * (1.f - t));
	}
	if (easing == Easing::CubicEaseInOut) {
		return t < 0.5f ? 4 * t * t * t : 1.f - ((-2.f * t + 2) * (-2.f * t + 2) * (-2.f * t + 2)) * 0.5f;
	}
	if (easing == Easing::QuintIn) {
		return std::pow(t, 5);
	}
	if (easing == Easing::QuintOut) {
		return 1.f - std::pow(1.f - t, 5);
	}
	if (easing == Easing::QuintInOut) {
		return t < 0.5 ? 16 * t * t * t * t * t : 1 - std::pow(-2 * t + 2, 5) * 0.5f;
	}
	if (easing == Easing::CircIn) {
		return 1.f - std::sqrt(1.f - std::pow(t, 2));
	}
	if (easing == Easing::CircOut) {
		return std::sqrt(1.f - std::pow(t - 1.f, 2));
	}
	if (easing == Easing::CircInOut) {
		return t < 0.5f ? (1.f - std::sqrt(1.f - std::pow(2 * t, 2))) * 0.5f : (std::sqrt(1.f - std::pow(-2.f * t + 2.f, 2)) + 1.f) * 0.5f;
	}
	if (easing == Easing::ElasticIn) {
		return t == 0.f ? 0.f : t == 1.f ? 1.f
										 : -std::pow(2, 10 * t - 10.f) * std::sin((t * 10.f - 10.75f) * (2 * PI) * 0.33f);
	}
	if (easing == Easing::ElasticOut) {
		return t == 0.f ? 0.f : t == 1 ? 1.f
									   : std::pow(2, -10.f * t) * std::sin((t * 10.f - 0.75f) * (2 * PI) * 0.33f) + 1.f;
	}
	if (easing == Easing::ElasticInOut) {
		return t == 0.f ? 0.f : t == 1.f ? 1.f
							: t < 0.5f	 ? -(std::pow(2, 20 * t - 10.f) * std::sin((20 * t - 11.125) * (2 * PI) / 4.5)) * 0.5f
										 : (std::pow(2, -20 * t + 10) * std::sin((20 * t - 11.125) * (2 * PI) / 4.5)) * 0.5f + 1;
	}
	if (easing == Easing::QuadIn) {
		return t * t;
	}
	if (easing == Easing::QuadOut) {
		return 1.f - (1.f - t) * (1.f - t);
	}
	if (easing == Easing::QuadInOut) {
		return t < 0.5f ? 2 * t * t : 1.f - std::pow(-2.f * t + 2.f, 2) * 0.5f;
	}
	if (easing == Easing::QuartIn) {
		return t * t * t * t;
	}
	if (easing == Easing::QuartOut) {
		return 1.f - std::pow(1.f - t, 4);
	}
	if (easing == Easing::QuartInOut) {
		return t < 0.5f ? 8 * t * t * t * t : 1.f - std::pow(-2.f * t + 2, 4) * 0.5f;
	}
	if (easing == Easing::ExpoIn) {
		return t == 0.f ? 0 : std::pow(2, 10 * t - 10);
	}
	if (easing == Easing::ExpoOut) {
		return t == 1.f ? 1.f : 1.f - std::pow(2, -10 * t);
	}
	if (easing == Easing::ExpoInOut) {
		return t == 0.f ? 0.f : t == 1.f ? 1.f
							: t < 0.5f	 ? std::pow(2, 20 * t - 10) * 0.5f
										 : (2 - std::pow(2, -20 * t + 10)) * 0.5f;
	}
	if (easing == Easing::BackIn) {
		float c1 = 1.70158;
		float c3 = c1 + 1;

		return c3 * t * t * t - c1 * t * t;
	}
	if (easing == Easing::BackOut) {
		float c1 = 1.70158;
		float c3 = c1 + 1;

		return 1.f + c3 * std::pow(t - 1, 3) + c1 * std::pow(t - 1, 2);
	}
	if (easing == Easing::BackInOut) {
		float c1 = 1.70158;
		float c2 = c1 * 1.525;

		return t < 0.5f ? (std::pow(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) * 0.5f : (std::pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) * 0.5f;
	}
	if (easing == Easing::BounceIn) {
		return 1.f - LerpMap(1.f - t, Easing::BounceOut);
	}
	if (easing == Easing::BounceOut) {
		float scaledTime = t / 1;

		if (scaledTime < (1 / 2.75)) {

			return 7.5625 * scaledTime * scaledTime;

		} else if (scaledTime < (2 / 2.75)) {

			float scaledTime2 = scaledTime - (1.5 / 2.75);
			return (7.5625 * scaledTime2 * scaledTime2) + 0.75;

		} else if (scaledTime < (2.5 / 2.75)) {

			float scaledTime2 = scaledTime - (2.25 / 2.75);
			return (7.5625 * scaledTime2 * scaledTime2) + 0.9375;

		} else {

			float scaledTime2 = scaledTime - (2.625 / 2.75);
			return (7.5625 * scaledTime2 * scaledTime2) + 0.984375;
		}
	}
	if (easing == Easing::BounceInOut) {
		return t < 0.5f ? (1.f - LerpMap(2 * t, Easing::BounceOut)) * 0.5f
						: (LerpMap((t * 2) - 1, Easing::BounceOut) * 0.5) + 0.5;
	}

	Debug::Error("Unreachable - easing.cpp:{}", __LINE__);
	return t;
}

float Utils::Lerp(float from, float to, float t, Easing easing) {
	t = LerpMap(t, easing);
	return from + ((to - from) * t);
}