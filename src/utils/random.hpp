#ifndef RANDOM_HPP
#define RANDOM_HPP
#pragma once

namespace Utils {
void Randomize();
int Rand();

int RandRange(int min, int max);
float RandFloat();
float RandRangeFloat(float min, float max);

} // namespace Utils

#endif // RANDOM_HPP