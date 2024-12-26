#include "argparser.hpp"

#include <vector>

#include "core/debug.hpp"

ArgParser::ArgParser() {}

void ArgParser::Parse(int argc, char const *argv[]) {
	if (argc <= 1) {
		return;
	}

	std::vector<std::string> args(argc);
	for (int i = 0; i < argc; i++) {
		args[i] = argv[i];
	}

	std::string cmd = args[1];
	if (cmd == "help") {
		printHelp();
		exit(0);
	}
	if (cmd == "version") {
		printVersion();
		exit(0);
	}
	if (cmd == "open") {
		if (argc <= 2) {
			Debug::Error("Failed to open. Project path not given. see sowa help");
		}
		_projectDirectory = args[2];
	}
}

void ArgParser::printHelp() {
	Debug::Log("Usage:");
	Debug::Log("./sowa <commands> [arguments]");
	Debug::Log("The commands are:");
	Debug::Log("  help             print available commands");
	Debug::Log("  version          print sowa version");
	Debug::Log("  open <directory> opens specific directory as a project file");
}

void ArgParser::printVersion() {
	Debug::Log("sowa version {}@{}", s_gitHash, s_gitBranch);
}