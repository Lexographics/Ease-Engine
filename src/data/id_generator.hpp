#ifndef ID_GENERATOR_HPP
#define ID_GENERATOR_HPP
#pragma once

#include "sowa.hpp"

#include "random_number_generator.hpp"
#include "utils/utils.hpp"

class UUIDGenerator {
  public:
	inline u64 Next() {
		return gen.U64();
	}

  private:
	RandomNumberGenerator gen;
};

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