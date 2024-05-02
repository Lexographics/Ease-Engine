#ifndef RESOURCE_HPP
#define RESOURCE_HPP
#pragma once

#include "sowa.hpp"

class Resource {
  public:
	Resource();
	Resource(RID rid);
	~Resource();

	inline RID GetRID() const { return _rid; }

  private:
	friend class ResourceRegistry;
	RID _rid = 0;
};

#endif // RESOURCE_HPP