#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP
#pragma once

#include <cstddef>
#include <filesystem>
#include <memory>
#include <vector>

struct FileDataInternal {
	std::vector<std::byte> buffer;
};
using FileData = std::shared_ptr<FileDataInternal>;

struct FileEntry {
	std::filesystem::path path = "";
	bool is_directory = false;
};

class FileSystem {
  public:
	virtual FileData Load(const char *path) = 0;
	virtual std::vector<FileEntry> ReadDirectory(const std::filesystem::path &path) { return std::vector<FileEntry>{}; }
};

class FolderFileSystem : public FileSystem {
  public:
	FolderFileSystem(const std::filesystem::path &path);

	FileData Load(const char *path) override;
	std::vector<FileEntry> ReadDirectory(const std::filesystem::path &path) override;

	std::filesystem::path GetPath(std::filesystem::path path);

  private:
	std::filesystem::path _basePath = "";
};

#endif // FILESYSTEM_HPP