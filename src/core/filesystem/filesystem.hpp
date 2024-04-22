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

class FileSystem {
  public:
	virtual FileData Load(const char *path) = 0;
};

class FolderFileSystem : public FileSystem {
  public:
	FolderFileSystem(const std::filesystem::path &path);

	FileData Load(const char *path) override;

  private:
	std::filesystem::path _basePath = "";
};

#endif // FILESYSTEM_HPP