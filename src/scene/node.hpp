#ifndef NODE_HPP
#define NODE_HPP
#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "core/serialize/document.hpp"
#include "sowa.hpp"
#include "utils/utils.hpp"

class Scene;

class Node {
  public:
	virtual ~Node() = default;

	virtual void Start() {}
	virtual void Update() {}

	virtual bool Serialize(Document &doc);
	virtual bool Deserialize(const Document &doc);

	virtual bool Copy(Node *dst);
	virtual void UpdateEditor() {}

	//
	inline NodeTypeID TypeID() const { return _typeid; }
	inline NodeID ID() const { return _id; }

	inline const std::string &Name() const { return _name; }
	inline const std::string &GetName() const { return _name; }
	inline void Rename(const std::string &name) { _name = name; }

	inline std::vector<Node *> GetChildren() { return _children; }
	inline Node *GetParent() { return _parent; }

	void RemoveChild(Node *child);
	void AddChild(Node *child);
	void Free();

	size_t GetChildCount();
	Node *GetChild(size_t index);

	Node *GetNode(const std::string nodePath, bool recursive = true);

	// If no scene is given, duplicates in current scene
	Node *Duplicate(Scene *scene = nullptr);

  private:
	// Internal hierarchy functions that does not modify other than the node passed
	void removeChild(Node *child);

	friend class Scene;
	friend class NodeDB;

	NodeTypeID _typeid = 0;
	NodeID _id = 0;

	std::string _name = "";

	Node *_parent = nullptr;
	std::vector<Node *> _children;

	Scene *_pScene = nullptr;
};

#endif // NODE_HPP