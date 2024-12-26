#ifndef ARGPARSER_HPP
#define ARGPARSER_HPP
#pragma once

#include <string>

class ArgParser {
  public:
	ArgParser();
	void Parse(int argc, char const *argv[]);

	inline const std::string &GetProjectDirectory() const { return _projectDirectory; }

  private:
	void printHelp();
	void printVersion();

  private:
	std::string _projectDirectory = "./";
};

#endif // ARGPARSER_HPP