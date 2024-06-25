#ifndef RANDOM_NUMBER_GENERATOR_HPP
#define RANDOM_NUMBER_GENERATOR_HPP
#pragma once

#include <random>

#include "sowa.hpp"

class RandomNumberGenerator {
  public:
	RandomNumberGenerator() : e1(_device()) {}

	inline i32 I32() {
		return std::uniform_int_distribution<i32>{INT32_MIN, INT32_MAX}(_device);
	}

	inline i64 I64() {
		return std::uniform_int_distribution<i64>{INT64_MIN, INT64_MAX}(_device);
	}

	inline u32 U32() {
		return std::uniform_int_distribution<u32>{0, UINT32_MAX}(_device);
	}

	inline u64 U64() {
		return std::uniform_int_distribution<u64>{0, UINT64_MAX}(_device);
	}

  private:
	std::random_device _device;
	std::mt19937 e1;
};

#endif // RANDOM_NUMBER_GENERATOR_HPP