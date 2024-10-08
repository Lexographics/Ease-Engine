#ifndef TWEEN_HPP
#define TWEEN_HPP
#pragma once

#include <functional>

#include "core/debug.hpp"
#include "math/math.hpp"
#include "utils/easing.hpp"

class Tween {
  public:
	Tween(float duration, Easing easing, const std::function<void(float)> &callback, const std::function<void()> &onFinish) {
		if (duration == 0.f) {
			// ERROR
			return;
		}
		_duration = duration;
		_easing = easing;
		_callback = callback;
		_onFinish = onFinish;
	}

	inline void Update(float delta) {
		if (_finished)
			return;

		_timePassed += delta;

		float t = Math::Clamp(_timePassed / _duration, 0.f, 1.f);

		// value can be out of [0-1] range on functions like elastic or bounce, value out of [0-1] range does not mean tween is finished
		float value = Utils::Lerp(0.f, 1.f, t, _easing);
		if (_callback) {
			_callback(value);
		}

		if (t >= 1.f) {
			if (_onFinish)
				_onFinish();
			_finished = true;
		}
	};

	inline void SetCallback(const std::function<void(float)> &callback) {
		_callback = callback;
	}

	inline void SetOnFinish(const std::function<void()> &onFinish) {
		_onFinish = onFinish;
	}

  private:
	float _timePassed = 0.f;
	float _duration = 1.f;
	Easing _easing = Easing::Linear;

	std::function<void(float)> _callback = nullptr;
	std::function<void()> _onFinish = nullptr;
	bool _finished = false;
};

#endif // TWEEN_HPP