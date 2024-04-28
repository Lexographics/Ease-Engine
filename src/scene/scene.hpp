#ifndef SCENE_HPP
#define SCENE_HPP
#pragma once

#include <sowa.hpp>
#include <string>
#include <unordered_map>

#include "node.hpp"
#include "node_db.hpp"

#include "data/id_generator.hpp"

class Scene {
  public:
	void Start();
	void Update();
	void Shutdown();

	Node *Create(NodeTypeID type, const std::string &name = "Object", NodeID id = 0);
	Node *Create(const char *typeName, const std::string &name = "Object", NodeID id = 0);

	bool HasNode(NodeID id);
	Node *GetNode(NodeID id);

  private:
	friend class Application;
	std::unordered_map<NodeID, Node *> _nodes;

	NodeDB *_nodeDB = nullptr;
};

#endif // SCENE_HPP