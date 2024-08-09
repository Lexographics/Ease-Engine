#ifndef FILE_SERVER_HPP
#define FILE_SERVER_HPP
#pragma once

#include <filesystem>
#include <vector>

#include "sowa.hpp"

struct FileData;
struct FileEntry;

class FileServer {
  public:
	virtual Ref<FileData> Load(const std::filesystem::path &path) = 0;
	virtual std::vector<FileEntry> ReadDirectory(const std::filesystem::path &path, bool recursive) { return std::vector<FileEntry>{}; };
};

class SaveableFileServer {
  public:
	virtual void GetSaveStream(const std::filesystem::path &path, std::ofstream &out) = 0;
};

#endif // FILE_SERVER_HPP