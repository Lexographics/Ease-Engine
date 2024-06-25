#ifndef TIME_HPP
#define TIME_HPP
#pragma once

#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

namespace Utils {

inline std::string GetTime(const std::string &format = "%Y-%m-%d") {
	std::stringstream date;
	std::time_t t = std::time(nullptr);
	std::tm tp = *std::localtime(&t);
	date << std::put_time(&tp, format.c_str());
	return date.str();
}

} // namespace Utils

#endif // TIME_HPP