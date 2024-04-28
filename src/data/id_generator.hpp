#ifndef ID_GENERATOR_HPP
#define ID_GENERATOR_HPP
#pragma once

#include "sowa.hpp"

template <typename T = u32>
class LinearIDGenerator {
  public:
	using counter_t = T;

	LinearIDGenerator() : _id(1) {}
	LinearIDGenerator(counter_t start) : _id(start) {}

	inline counter_t Next() {
		return _id++;
	}

  private:
	counter_t _id = 1;
};

#endif // ID_GENERATOR_HPP