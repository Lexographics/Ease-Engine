#include "filesystem.hpp"

#include <filesystem>
#include <fstream>

#include "core/debug.hpp"

PathData FileSystem::ResolvePath(const std::string &path) {
	PathData pathData;

	auto data = Utils::Split(path, "://");
	if (data.size() == 2) {
		pathData.scheme = data[0];
		pathData.path = data[1];
	}

	return pathData;
}

FolderFileSystem::FolderFileSystem(const std::filesystem::path &path) {
	_basePath = path;
}

FileData FolderFileSystem::Load(const char *path) {
	FileData data = std::make_shared<FileDataInternal>();

	std::ifstream file{GetPath(path), std::ios::binary};
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
	std::filesystem::path dir = GetPath(path);

	if (std::filesystem::is_directory(dir))
		for (const auto &dirEntry : std::filesystem::directory_iterator(dir)) {
			entries.push_back(FileEntry{
				.path = std::filesystem::relative(dirEntry.path(), _basePath),
				.is_directory = dirEntry.is_directory()});
		}

	return entries;
}

std::filesystem::path FolderFileSystem::GetPath(std::filesystem::path path) {
	auto pathData = ResolvePath(path);

	if (pathData.scheme == "res") {
		return _basePath / pathData.path;
	}

	return path;
}