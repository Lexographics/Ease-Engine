#include "resource_registry.hpp"

#include <iostream>

#include "data/id_generator.hpp"

Resource *ResourceRegistry::GetResource(RID rid) {
	return _resources[rid];
}

void ResourceRegistry::AddResource(Resource *res) {
	if (res->GetRID() == 0) {
		static UUIDGenerator gen;
		res->_rid = gen.Next();
	}

	_resources[res->GetRID()] = res;
}

void ResourceRegistry::RemoveResource(Resource *res) {
}