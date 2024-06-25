#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP
#pragma once

#include <cstddef>
#include <filesystem>
#include <memory>
#include <unordered_map>
#include <vector>

struct FileDataInternal {
	std::vector<std::byte> buffer;
};
using FileData = std::shared_ptr<FileDataInternal>;

struct FileEntry {
	std::filesystem::path path = "";
	bool is_directory = false;

	bool operator<(const FileEntry &other) {
		return path.string() < other.path.string();
	}
};

struct PathData {
	std::string scheme;
	std::string path;
};

class FileServer {
  public:
	virtual FileData Load(const std::filesystem::path &path) = 0;
	virtual std::vector<FileEntry> ReadDirectory(const std::filesystem::path &path) { return std::vector<FileEntry>{}; };
};

class SaveableFileServer {
  public:
	virtual void GetSaveStream(const std::filesystem::path &path, std::ofstream &out) = 0;
};

class FileSystem {
  public:
	void RegisterFileServer(const char *scheme, FileServer *server);
	FileServer *GetFileServer(const char *scheme);
	bool HasFileServer(const char *scheme);
	PathData ResolvePath(const std::string &path);

	FileData Load(const std::filesystem::path &path);
	std::vector<FileEntry> ReadDirectory(const std::filesystem::path &path);

	FileServer *NewFolderFileServer(const char *scheme, const std::filesystem::path &path);

  private:
	std::unordered_map<std::string, FileServer *> _fileServers;
};

#endif // FILESYSTEM_HPP