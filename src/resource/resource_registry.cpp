#include "resource_registry.hpp"

#include <iostream>

#include "data/id_generator.hpp"

Resource *ResourceRegistry::GetResource(RID rid) {
	return _resources[rid];
}

const std::unordered_map<RID, Resource *> ResourceRegistry::GetResources() {
	return _resources;
}

void ResourceRegistry::AddResource(Resource *res, RID rid) {
	if (rid != 0)
		res->_rid = rid;

	if (res->GetRID() == 0) {
		static UUIDGenerator gen;
		res->_rid = gen.NextI32();
	}

	_resources[res->GetRID()] = res;
}

void ResourceRegistry::RemoveResource(Resource *res) {
	// TODO: Implement
}

void ResourceRegistry::RemoveResourceByID(RID rid) {
	// TODO: Implement
}