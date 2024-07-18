#include "resource_locker.hpp"

#include "core/application.hpp"
#include "resource_registry.hpp"

Resource *ResourceLocker::CreateResource(TypeID type, RID rid) {
	Resource *res = App().GetResourceRegistry().CreateResource(type, rid);
	_resources.push_back(res->GetRID());
	return res;
}

Resource *ResourceLocker::CreateResource(const char *typeName, RID rid) {
	Resource *res = App().GetResourceRegistry().CreateResource(typeName, rid);
	_resources.push_back(res->GetRID());
	return res;
}

void ResourceLocker::Clear() {
	_resources.clear();
}