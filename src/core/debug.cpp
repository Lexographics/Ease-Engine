#include "debug.hpp"

#include <string>

#include "core/application.hpp"

static std::deque<Debug::LogMessage> s_Lines;

void Debug::Internal::PushLine(const std::string &line, LogSeverity severity) {
	if (severity == LogSeverity::Log) {
		std::cout << "\033[0;32m";
	} else if (severity == LogSeverity::Info) {
		std::cout << "\033[0;36m";
	} else if (severity == LogSeverity::Warn) {
		std::cout << "\033[0;33m";
	} else if (severity == LogSeverity::Error) {
		std::cout << "\033[0;31m";
	}

	std::cout << line << "\033[0m\n";

	s_Lines.push_back(Debug::LogMessage{line, severity});
}

const std::deque<Debug::LogMessage> &Debug::GetLines() {
	return s_Lines;
}