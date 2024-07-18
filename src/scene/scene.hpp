#ifndef SCENE_HPP
#define SCENE_HPP
#pragma once

#include <filesystem>
#include <set>
#include <sowa.hpp>
#include <string>
#include <unordered_map>
#include <vector>

#include "node.hpp"
#include "node_db.hpp"

#include "data/id_generator.hpp"
#include "resource/resource_locker.hpp"

class Scene {
  public:
	~Scene();

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

	void SetCurrentCamera2D(NodeID id);
	NodeID GetCurrentCamera2D();
	glm::mat4 GetMatrix2D();

	void FreeNode(NodeID id);

	const std::filesystem::path &GetFilepath();

	bool SaveToFile(const char *path = nullptr);
	bool LoadFromFile(const char *path);

	void Clear();
	static void Copy(Scene *src, Scene *dst);

	inline ResourceLocker &GetResourceLocker() { return _resourceLocker; }

  private:
	void freeNode(NodeID id);

  private:
	friend class Application;
	friend class Editor;
	std::unordered_map<NodeID, Node *> _nodes;
	std::set<Node *> _nodeIter;

	std::vector<NodeID> _freeList;

	Node *_root = nullptr;
	NodeID _currentCamera2D = 0;
	std::vector<std::string> _scripts;

	NodeDB *_nodeDB = nullptr;

	std::filesystem::path _scenePath = "";

	ResourceLocker _resourceLocker;
};

#endif // SCENE_HPP