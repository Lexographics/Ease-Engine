#ifndef SW_SCRIPT_SERVER_HXX
#define SW_SCRIPT_SERVER_HXX
#pragma once

#include <angelscript.h>
#include <string>

#include "core/error/error.hxx"

class ScriptServer {
  public:
	static ScriptServer &get();

	void BeginBuild();
	ErrorCode LoadScriptFile(std::string path);
	void EndBuild();

  private:
	void register_script_behaviour();

	ScriptServer();
	~ScriptServer();
};

#endif // SW_SCRIPT_SERVER_HXX