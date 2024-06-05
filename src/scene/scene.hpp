#ifndef SCENE_HPP
#define SCENE_HPP
#pragma once

#include <filesystem>
#include <set>
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

	// TODO: if root has parent, deattach from its parent
	void SetRoot(Node *node);
	Node *GetRoot();

	const std::filesystem::path &GetFilepath();

	bool SaveToFile(const char *path = nullptr);
	bool LoadFromFile(const char *path);

  private:
	friend class Application;
	std::unordered_map<NodeID, Node *> _nodes;
	std::set<Node *> _nodeIter;

	Node *_root = nullptr;

	NodeDB *_nodeDB = nullptr;

	std::filesystem::path _scenePath = "";
};

#endif // SCENE_HPP