#ifndef DATA_FILE_SERVER
#define DATA_FILE_SERVER
#pragma once

#include <filesystem>
#include <unordered_map>

#include "file_server.hpp"

struct FileData;

class DataFileServer : public FileServer {
  public:
	DataFileServer &AddFile(const char *path, Ref<FileData> data);

	Ref<FileData> Load(const std::filesystem::path &path) override;

  private:
	std::unordered_map<std::string, Ref<FileData>> _files;
};

#endif // DATA_FILE_SERVER