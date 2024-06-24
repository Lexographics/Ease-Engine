#ifndef SCRIPT_SERVER_HPP
#define SCRIPT_SERVER_HPP
#pragma once

#include <vector>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

class ScriptServer {
  public:
	void Init();

	void CallStart();
	void CallUpdate();

	void LoadScript(const char *path);

  private:
	lua_State *state = nullptr;

	// reference in LUA_REGISTRYINDEX
	std::vector<int> _startFuncs;
	std::vector<int> _updateFuncs;
};

#endif // SCRIPT_SERVER_HPP