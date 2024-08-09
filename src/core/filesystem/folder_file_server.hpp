#ifndef FOLDER_FILE_SERVER
#define FOLDER_FILE_SERVER
#pragma once

#include <filesystem>

#include "file_server.hpp"

struct FileData;
class FileSystem;

class FolderFileServer : public FileServer, public SaveableFileServer {
  public:
	FolderFileServer(FileSystem *fs, const char *scheme, const std::filesystem::path &path);

	Ref<FileData> Load(const std::filesystem::path &path) override;
	std::vector<FileEntry> ReadDirectory(const std::filesystem::path &path, bool recursive) override;

	/* Below functions are NOT resolved ( res://file.png ) */
	void GetSaveStream(const std::filesystem::path &path, std::ofstream &out) override;

	bool IsDirectory(const std::filesystem::path &path);
	bool Remove(const std::filesystem::path &path);
	uintmax_t RemoveAll(const std::filesystem::path &path);
	bool Rename(const std::filesystem::path &path, const std::string &name);
	bool Create(const std::filesystem::path &path);

  private:
	friend class Editor;
	std::filesystem::path GetPath(const std::filesystem::path &path);

  private:
	FileSystem *_fs = nullptr;
	std::string _scheme = "";
	std::filesystem::path _basePath = "";
};

#endif // FOLDER_FILE_SERVER