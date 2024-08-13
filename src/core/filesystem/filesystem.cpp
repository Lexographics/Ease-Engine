#include "filesystem.hpp"

#include <filesystem>
#include <fstream>

#include "core/debug.hpp"

Ref<FileData> FileData::New() {
	return std::make_shared<FileData>();
}

Ref<FileData> FileData::NewStatic(std::byte *data, size_t size) {
	Ref<FileData> filedata = FileData::New();
	filedata->dynamic = false;
	filedata->data = data;
	filedata->size = size;
	return filedata;
}

void FileSystem::RegisterFileServer(const char *scheme, FileServer *server) {
	_fileServers[scheme] = server;
}

FileServer *FileSystem::GetFileServer(const char *scheme) {
	if (_fileServers.find(std::string(scheme)) == _fileServers.end()) {
		return nullptr;
	}
	return _fileServers[scheme];
}

bool FileSystem::HasFileServer(const char *scheme) {
	return _fileServers.find(std::string(scheme)) != _fileServers.end();
}

PathData FileSystem::ResolvePath(const std::string &path) {
	PathData pathData;

	auto data = Utils::Split(path, "://");
	if (data.size() == 2) {
		pathData.scheme = data[0];
		pathData.path = data[1];
	} else if (data.size() == 1) {
		// starts with ://
		if (path[0] == ':' && path[1] == '/' && path[2] == '/') {
			pathData.scheme = "";
			pathData.path = data[0];
		} else {
			pathData.scheme = data[0];
			pathData.path = "";
		}
	}

	auto index = pathData.path.find_first_not_of('/');
	if (index != std::string::npos)
		pathData.path = pathData.path.substr(index);
	return pathData;
}

Ref<FileData> FileSystem::Load(const std::filesystem::path &path) {
	PathData data = ResolvePath(path);
	if (!HasFileServer(data.scheme.c_str()) || _fileServers[data.scheme] == nullptr)
		return nullptr;

	return _fileServers[data.scheme]->Load(data.path);
}

std::vector<FileEntry> FileSystem::ReadDirectory(const std::filesystem::path &path, bool recursive) {
	PathData data = ResolvePath(path);
	if (!HasFileServer(data.scheme.c_str()) || _fileServers[data.scheme] == nullptr)
		return std::vector<FileEntry>();

	return _fileServers[data.scheme]->ReadDirectory(data.path, recursive);
}

FolderFileServer *FileSystem::NewFolderFileServer(const char *scheme, const std::filesystem::path &path) {
	return new FolderFileServer(this, scheme, path);
}

DataFileServer *FileSystem::NewDataFileServer() {
	return new DataFileServer();
}

bool FileSystem::TrySaveFile(const std::string &path, Ref<FileData> file) {
	if (!file) {
		return false;
	}

	PathData pathData = ResolvePath(path);
	FileServer *fs = GetFileServer(pathData.scheme.c_str());
	if (fs == nullptr) {
		return false;
	}

	SaveableFileServer *saveFs = dynamic_cast<SaveableFileServer *>(fs);
	if (saveFs == nullptr) {
		return false;
	}

	std::ofstream out;
	saveFs->GetSaveStream(path, out);
	if (!out.good()) {
		return false;
	}

	out.write(reinterpret_cast<const char *>(file->Data()), file->Size());
	return true;
}