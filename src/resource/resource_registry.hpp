#ifndef RESOURCE_REGISTRY
#define RESOURCE_REGISTRY
#pragma once

#include <unordered_map>

#include "core/resource.hpp"
#include "sowa.hpp"

class ResourceRegistry {
  public:
	Resource *GetResource(RID rid);
	void AddResource(Resource *res);
	void RemoveResource(Resource *res);

  private:
	std::unordered_map<RID, Resource *> _resources;
};

#endif // RESOURCE_REGISTRY