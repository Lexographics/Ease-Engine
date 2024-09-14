#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP
#pragma once

#include <cstddef>
#include <filesystem>
#include <memory>
#include <unordered_map>
#include <vector>

#include "sowa.hpp"

#include "data_file_server.hpp"
#include "file_server.hpp"
#include "folder_file_server.hpp"
#include "zip_file_server.hpp"

struct FileData {
  public:
	FileData() = default;
	~FileData() = default;

	static Ref<FileData> New();
	// Returns a FileData that contains static data. Dynamic buffer can still be accessed. but .Data() and .Size() functions will return static data
	static Ref<FileData> NewStatic(std::byte *data, size_t size);

	std::byte *Data() {
		if (dynamic)
			return buffer.data();
		return data;
	}

	size_t Size() {
		if (dynamic)
			return buffer.size();
		return size;
	}

	std::vector<std::byte> &Buffer() {
		return buffer;
	}

  private:
	std::byte *data;
	size_t size;

	bool dynamic = true;
	std::vector<std::byte> buffer;
};

struct FileEntry {
	std::filesystem::path path = "";
	bool is_directory = false;

	bool operator<(const FileEntry &other) const {
		return path.string() < other.path.string();
	}
};

struct PathData {
	std::string scheme;
	std::string path;
};

class FileSystem {
  public:
	void RegisterFileServer(const char *scheme, FileServer *server);
	FileServer *GetFileServer(const char *scheme);
	bool HasFileServer(const char *scheme);
	PathData ResolvePath(const std::string &path);

	Ref<FileData> Load(const std::filesystem::path &path);
	std::vector<FileEntry> ReadDirectory(const std::filesystem::path &path, bool recursive = false);

	FolderFileServer *NewFolderFileServer(const char *scheme, const std::filesystem::path &path);
	DataFileServer *NewDataFileServer();
	ZipFileServer *NewZipFileServer(const char *scheme, const std::filesystem::path &path);

	bool TrySaveFile(const std::string &path, Ref<FileData> file);

  private:
	std::unordered_map<std::string, FileServer *> _fileServers;
};

#endif // FILESYSTEM_HPP