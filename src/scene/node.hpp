#ifndef NODE_HPP
#define NODE_HPP
#pragma once

#include <string>

#include "sowa.hpp"

class Node {
  public:
	//
	inline NodeTypeID TypeID() const { return _typeid; }
	inline NodeID ID() const { return _id; }

	inline const std::string &Name() const { return _name; }
	inline void Rename(const std::string &name) { _name = name; }

  private:
	friend class Scene;
	friend class NodeDB;

	NodeTypeID _typeid = 0;
	NodeID _id = 0;

	std::string _name = "";
};

#endif // NODE_HPP