#ifndef SCRIPT_SERVER_HPP
#define SCRIPT_SERVER_HPP
#pragma once

#include "sowa.hpp"

#include <memory>
#include <string>
#include <vector>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

using NodeScriptID = int;

struct NodeScriptInstance {
	NodeScriptID scriptID;
	std::string path;
};

class NodeScriptRef {
  public:
	~NodeScriptRef();

	void Clear();

  private:
	friend class ScriptServer;

	std::vector<NodeScriptInstance> _scripts;
};

struct ScriptServerData;
class Node;

class ScriptServer {
  public:
	ScriptServer();
	~ScriptServer();

	void Init();

	void CallStart();
	void CallUpdate();

	// void LoadScript(const char *path);

	bool LoadNodeScript(const char *path);

	void NewNodeScript(Node *node, NodeScriptRef &ref, const std::string &scriptName);
	void DestroyNodeScript(NodeScriptID id);

	int PushModule(const char *path);

  private:
	lua_State *state = nullptr;

	std::unique_ptr<ScriptServerData> data;
};

#endif // SCRIPT_SERVER_HPP