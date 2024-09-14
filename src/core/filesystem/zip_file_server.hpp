#ifndef ZIP_FILE_SERVER_HPP
#define ZIP_FILE_SERVER_HPP
#pragma once

#include <filesystem>

#include "data/zip_reader.hpp"
#include "file_server.hpp"

class ZipFileServer : public FileServer {
  public:
	ZipFileServer(const char *scheme, const std::filesystem::path &path);

	Ref<FileData> Load(const std::filesystem::path &path) override;
	// TODO: ZipFileServer ReadDirectory only returns files and skips directories, unlike FolderFileServer
	std::vector<FileEntry> ReadDirectory(const std::filesystem::path &path, bool recursive) override;

  private:
	ZipReader _reader;
	std::string _scheme = "";
};

#endif // ZIP_FILE_SERVER_HPP