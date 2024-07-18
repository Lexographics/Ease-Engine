#ifndef RESOURCE_REGISTRY
#define RESOURCE_REGISTRY
#pragma once

#include <functional>
#include <unordered_map>

#include "core/resource.hpp"
#include "sowa.hpp"

struct ResourceAllocate {
	std::function<Resource *()> createFunc;
	std::function<void(Resource *)> destroyFunc;
};

class ResourceRegistry {
  public:
	Resource *GetResource(RID rid);
	const std::unordered_map<RID, Resource *> GetResources();
	void AddResource(Resource *res, RID rid = 0);
	void RemoveResource(Resource *res);
	void RemoveResourceByID(RID rid);

	template <typename T>
	void AddResourceType(const char *name) {
		ResourceAllocate &alloc = _allocators[typeid(T).hash_code()];
		alloc.createFunc = []() -> Resource * {
			return new T;
		};
		alloc.destroyFunc = [](Resource *res) {
			delete reinterpret_cast<T *>(res);
		};
		_typenames[std::string(name)] = typeid(T).hash_code();
		_typeIds[typeid(T).hash_code()] = std::string(name);
	}

	Resource *CreateResource(TypeID type, RID rid = 0) {
		auto fn = _allocators[type].createFunc;
		if (fn) {
			Resource *res = fn();
			AddResource(res, rid);
			return res;
		}
		return nullptr;
	}

	Resource *CreateResource(const char *typeName, RID rid = 0) {
		return CreateResource(_typenames[std::string(typeName)], rid);
	}

	template <typename T>
	T *NewResource(RID rid = 0) {
		return dynamic_cast<T *>(CreateResource(typeid(T).hash_code(), rid));
	}

	std::string GetTypeName(TypeID id) {
		return _typeIds[id];
	}

  private:
	std::unordered_map<RID, Resource *> _resources;

	std::unordered_map<TypeID, ResourceAllocate> _allocators;
	std::unordered_map<std::string, TypeID> _typenames;
	std::unordered_map<TypeID, std::string> _typeIds;
};

#endif // RESOURCE_REGISTRY