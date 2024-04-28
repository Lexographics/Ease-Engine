#ifndef NODE_DB_HPP
#define NODE_DB_HPP
#pragma once

#include <string>
#include <unordered_map>

#include "allocator.hpp"
#include "node.hpp"
#include "sowa.hpp"

#include "data/id_generator.hpp"

class NodeDB {
  public:
	template <typename T>
	NodeTypeID NewNodeType(const char *name) {
		NodeTypeID id = _nodeTypeIdGen.Next();
		_allocators[id] = NodeAllocator::Get<T>();

		_typeids[name] = id;
		_typenames[id] = name;

		return id;
	}

	Node *Create(NodeTypeID type);
	void Destroy(Node *node);

	NodeTypeID GetNodeTypeID(const std::string &typeName);
	const char *GetNodeTypename(NodeTypeID typeId);

  private:
	std::unordered_map<NodeTypeID, NodeAllocator> _allocators;

	std::unordered_map<std::string, NodeTypeID> _typeids;
	std::unordered_map<NodeTypeID, std::string> _typenames;

	LinearIDGenerator<NodeTypeID> _nodeTypeIdGen;
};

#endif // NODE_DB_HPP