#ifndef STRING_HPP
#define STRING_HPP
#pragma once

#include <string>
#include <vector>

namespace Utils {
std::vector<std::string> Split(std::string str, std::string delimiter);
} // namespace Utils

#endif // STRING_HPP