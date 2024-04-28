#include "node_db.hpp"

Node *NodeDB::Create(NodeTypeID type) {
	Node *node = _allocators[type].Create();
	if (!node)
		return nullptr;
	node->_typeid = type;
	return node;
}

void NodeDB::Destroy(Node *node) {
	if (!node)
		return;
	_allocators[node->TypeID()].Destroy(node);
}

NodeTypeID NodeDB::GetNodeTypeID(const std::string &typeName) {
	return _typeids[typeName];
}

const char *NodeDB::GetNodeTypename(NodeTypeID typeId) {
	return _typenames[typeId].c_str();
}