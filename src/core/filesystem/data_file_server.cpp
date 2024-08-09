#include "data_file_server.hpp"

#include "filesystem.hpp"

DataFileServer &DataFileServer::AddFile(const char *path, Ref<FileData> data) {
	_files[path] = data;
	return *this;
}

Ref<FileData> DataFileServer::Load(const std::filesystem::path &path) {
	if (_files.find(path.string()) == _files.end())
		return nullptr;

	return _files[path.string()];
}