#ifndef _E_RESOURCELOADER_HPP__
#define _E_RESOURCELOADER_HPP__
#pragma once

#include "sowa.hpp"
#include "stlpch.hpp"
#include "utils/file.hpp"
#include "resource.hpp"
#include "serialize/serializer.hpp"

namespace sowa {
template <typename T>
struct ResourceLoaderImpl {
	Reference<T> Load(const unsigned char *data, size_t size);
	Reference<T> LoadFromFile(const char *path); // Should only be available on necessary resource types.
	bool Unload(Reference<T> resource);
};

class ResourceLoader {
  public:
	static ResourceLoader &get_singleton() {
		static ResourceLoader l;
		return l;
	}

	template <typename T>
	Reference<T> LoadResourceFromMemory(const unsigned char *data, size_t size) {
		if (size > 0) {
			ResourceLoaderImpl<T> loader;
			return loader.Load(data, size);
		}
		return nullptr;
	}

	template <typename T>
	Reference<T> LoadResource(const std::string &path) {
		std::vector<unsigned char> data = sowa::File::GetFileContent(path.c_str());
		if (data.size() > 0) {
			ResourceLoaderImpl<T> loader;
			return loader.Load(data.data(), data.size());
		}
		return nullptr;
	}

	// Todo: void Poll() that hot reloads resources with Unload() -> Load()

  private:
	ResourceLoader() = default;
	~ResourceLoader() = default;
};
} // namespace sowa

#endif