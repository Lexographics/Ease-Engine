#ifndef RESOURCE_REGISTRY
#define RESOURCE_REGISTRY
#pragma once

#include <unordered_map>

#include "core/resource.hpp"
#include "sowa.hpp"

class ResourceRegistry {
  public:
	Resource *GetResource(RID rid);
	const std::unordered_map<RID, Resource *> GetResources();
	void AddResource(Resource *res, RID rid = 0);
	void RemoveResource(Resource *res);

  private:
	std::unordered_map<RID, Resource *> _resources;
};

#endif // RESOURCE_REGISTRY