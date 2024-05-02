#include "scene.hpp"

void Scene::Start() {
}
void Scene::Update() {
	for (Node *node : _nodeIter) {
		node->Update();
	}
}
void Scene::Shutdown() {
}

Node *Scene::Create(NodeTypeID type, const std::string &name, NodeID id) {
	static LinearIDGenerator<NodeID> gen;

	Node *node = _nodeDB->Create(type);
	if (!node)
		return nullptr;

	NodeID nodeId = (id != 0 && !HasNode(id)) ? id : gen.Next();
	node->_id = nodeId;
	node->Rename(name);

	_nodes[nodeId] = node;
	_nodeIter.insert(node);
	return node;
}

Node *Scene::Create(const char *typeName, const std::string &name, NodeID id) {
	return Create(_nodeDB->GetNodeTypeID(typeName), name, id);
}

bool Scene::HasNode(NodeID id) {
	return _nodes.find(id) != _nodes.end();
}

Node *Scene::GetNode(NodeID id) {
	if (!HasNode(id))
		return nullptr;
	return _nodes[id];
}