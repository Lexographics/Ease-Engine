#include "filesystem.hpp"

#include <filesystem>
#include <fstream>

FolderFileSystem::FolderFileSystem(const std::filesystem::path &path) {
	_basePath = path;
}

FileData FolderFileSystem::Load(const char *path) {
	FileData data = std::make_shared<FileDataInternal>();

	std::ifstream file{_basePath / std::filesystem::path(path), std::ios::binary};
	if (!file.good())
		return nullptr;

	file.seekg(0, std::ios::end);
	int size = static_cast<int>(file.tellg());
	file.seekg(0, std::ios::beg);

	data->buffer.resize(size);
	file.read(reinterpret_cast<char *>(data->buffer.data()), static_cast<long>(size));

	return data;
}