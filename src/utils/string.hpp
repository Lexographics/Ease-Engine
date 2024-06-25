#ifndef STRING_HPP
#define STRING_HPP
#pragma once

#include <string>
#include <vector>

#include "fmt/core.h"

namespace Utils {
std::vector<std::string> Split(std::string str, std::string delimiter);

template <typename... Args>
std::string Format(const std::string &format, Args... args) {
	return fmt::format(format, args...);
}

int FormatArgCount(const std::string &fmt);

} // namespace Utils

#endif // STRING_HPP