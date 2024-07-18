#ifndef RESOURCE_LOCKER_HPP
#define RESOURCE_LOCKER_HPP
#pragma once

#include "core/resource.hpp"

#include <vector>

class ResourceLocker {
  public:
	Resource *CreateResource(TypeID type, RID rid = 0);
	Resource *CreateResource(const char *typeName, RID rid = 0);

	void Clear();

	inline const std::vector<RID> &GetResources() const { return _resources; }

  private:
	std::vector<RID> _resources;
};

#endif // RESOURCE_LOCKER_HPP