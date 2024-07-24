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
	EDITOR_NODE_BASE()
  public:
	virtual ~Node() = default;

	virtual void Start() {}
	virtual void Update() {}
	virtual void Exit() {}

	virtual bool Serialize(Document &doc);
	virtual bool Deserialize(const Document &doc);

	virtual bool Copy(Node *dst);

	//
	inline NodeTypeID TypeID() const { return _typeid; }
	inline NodeID ID() const { return _id; }
	inline std::string GetID() const { return std::to_string(_id); }

	inline const std::string &Name() const { return _name; }
	inline const std::string &GetName() const { return _name; }
	inline void Rename(const std::string &name) { _name = name; }
	inline const std::vector<std::string> &Groups() const { return _groups; }
	inline bool IsInGroup(const std::string &group) {
		return std::find(_groups.begin(), _groups.end(), group) != _groups.end();
	}
	inline void AddGroup(const std::string &group) {
		if (IsInGroup(group))
			return;

		_groups.push_back(group);
	}
	inline void RemoveGroup(const std::string &group) {
		for (size_t i = 0; i < _groups.size();) {
			if (_groups[i] == group) {
				_groups.erase(_groups.begin() + i);
				return;
			}
			i++;
		}
	}

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

	bool IsHovered();

  private:
	// Internal hierarchy functions that does not modify other than the node passed
	void removeChild(Node *child);

	friend class Scene;
	friend class NodeDB;

	NodeTypeID _typeid = 0;
	NodeID _id = 0;

	std::string _name = "";
	std::vector<std::string> _groups;

	Node *_parent = nullptr;
	std::vector<Node *> _children;

	Scene *_pScene = nullptr;
};

#endif // NODE_HPP