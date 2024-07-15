#ifndef TIMER_HPP
#define TIMER_HPP
#pragma once

#include "eventpp/callbacklist.h"

class Timer {
  public:
	Timer() = default;
	Timer(float timeout, bool autoStart = false) : _timeout(timeout), _started(autoStart) {
	}

	inline void Start() {
		_started = true;
	}

	inline void Pause() {
		_started = false;
	}

	inline void Stop() {
		_started = false;
		_timePassed = 0.f;
	}

	inline void Update(float delta) {
		if (!_started) {
			return;
		}

		_timePassed += delta;
		if (_timePassed > _timeout) {
			_onTimeout();
			Stop();
		}
	}

	inline void OnTimeout(const std::function<void()> &func) {
		_onTimeout.append(func);
	}

  private:
	eventpp::CallbackList<void()> _onTimeout;

	float _timePassed = 0.f;
	float _timeout = 1.f;
	bool _started = false;
};

#endif // TIMER_HPP