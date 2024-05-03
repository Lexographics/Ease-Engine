#ifndef DEBUGGER_HPP
#define DEBUGGER_HPP
#pragma once

#include <deque>
#include <iostream>

#include "fmt/core.h"

#include "utils/utils.hpp"

namespace Debug {

enum class LogSeverity {
	Default = 0,
	Log,
	Info,
	Warn,
	Error,
};

struct LogMessage {
	std::string message;
	LogSeverity severity;

	LogMessage(const std::string &message, LogSeverity severity) : message(message), severity(severity) {}
};

namespace Internal {
void PushLine(const std::string &line, LogSeverity severity);

template <typename... Args>
void Print(const std::string &format, const char *severityText, LogSeverity severity, Args... args) {
	std::string time = Utils::GetTime("%Y-%m-%d %H:%M:%S");
	PushLine(Utils::Format("[{}]{} {}", time, severityText, Utils::Format(format, args...)), severity);
}
} // namespace Internal

const std::deque<LogMessage> &GetLines();

template <typename... Args>
void Print(const std::string &format, Args... args) {
	Internal::Print(format, "", LogSeverity::Default, args...);
}

template <typename... Args>
void Log(const std::string &format, Args... args) {
	Internal::Print(format, " [LOG]", LogSeverity::Log, args...);
}

template <typename... Args>
void Info(const std::string &format, Args... args) {
	Internal::Print(format, " [INFO]", LogSeverity::Info, args...);
}

template <typename... Args>
void Warn(const std::string &format, Args... args) {
	Internal::Print(format, " [WARN]", LogSeverity::Warn, args...);
}

template <typename... Args>
void Error(const std::string &format, Args... args) {
	Internal::Print(format, " [ERROR]", LogSeverity::Error, args...);
}
} // namespace Debug

#endif // DEBUGGER_HPP