#ifndef NODE_DB_HPP
#define NODE_DB_HPP
#pragma once

#include <string>
#include <unordered_map>

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}
#include <LuaBridge/LuaBridge.h>

#include "allocator.hpp"
#include "node.hpp"
#include "sowa.hpp"

#include "data/id_generator.hpp"

struct NodeType {
	std::string name = "";
	NodeTypeID extends = 0;
	const void *scriptKey = nullptr;
};

class NodeDB {
  public:
	template <typename T>
	NodeTypeID NewNodeType(const char *name, NodeTypeID extends) {
		NodeTypeID id = _nodeTypeIdGen.Next();
		_allocators[id] = NodeAllocator::Get<T>();

		_types[id] = NodeType{
			.name = name,
			.extends = extends,
			.scriptKey = luabridge::detail::getClassRegistryKey<T>(),
		};
		_typeids[name] = id;

		return id;
	}

	Node *Create(NodeTypeID type);
	void Destroy(Node *node);

	NodeTypeID GetNodeTypeID(const std::string &typeName);
	const char *GetNodeTypename(NodeTypeID typeId);
	const NodeType &GetNodeType(NodeTypeID typeId);
	const std::unordered_map<NodeTypeID, NodeType> &GetNodeTypes();

  private:
	std::unordered_map<NodeTypeID, NodeAllocator> _allocators;

	std::unordered_map<std::string, NodeTypeID> _typeids;
	std::unordered_map<NodeTypeID, NodeType> _types;

	LinearIDGenerator<NodeTypeID> _nodeTypeIdGen;
};

#endif // NODE_DB_HPP