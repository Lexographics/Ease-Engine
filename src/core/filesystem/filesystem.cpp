#include "filesystem.hpp"

#include <filesystem>
#include <fstream>

#include "core/debug.hpp"

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

std::vector<FileEntry> FolderFileSystem::ReadDirectory(const std::filesystem::path &path) {
	std::vector<FileEntry> entries;
	std::filesystem::path dir = Utils::Format("{}/{}", _basePath.string(), path.string());

	if (std::filesystem::is_directory(dir))
		for (const auto &dirEntry : std::filesystem::directory_iterator(dir)) {
			entries.push_back(FileEntry{
				.path = std::filesystem::relative(dirEntry.path(), _basePath),
				.is_directory = dirEntry.is_directory()});
		}

	return entries;
}

std::filesystem::path FolderFileSystem::GetPath(std::filesystem::path path) {
	return _basePath / path;
}