#include "folder_file_server.hpp"

#include <algorithm>
#include <fstream>

#include "core/debug.hpp"
#include "filesystem.hpp"
#include "utils/utils.hpp"

FolderFileServer::FolderFileServer(FileSystem *fs, const char *scheme, const std::filesystem::path &path) {
	_fs = fs;
	_scheme = scheme;
	_basePath = path;
}

Ref<FileData> FolderFileServer::Load(const std::filesystem::path &path) {
	Ref<FileData> data = FileData::New();

	std::ifstream file{GetPath(path), std::ios::binary};
	if (!file.good())
		return nullptr;

	file.seekg(0, std::ios::end);
	int size = static_cast<int>(file.tellg());
	file.seekg(0, std::ios::beg);

	data->Buffer().resize(size);
	file.read(reinterpret_cast<char *>(data->Buffer().data()), static_cast<long>(size));

	return data;
}

std::vector<FileEntry> FolderFileServer::ReadDirectory(const std::filesystem::path &path, bool recursive) {
	std::vector<FileEntry> entries;
	std::filesystem::path dir = GetPath(path);

	if (std::filesystem::is_directory(dir)) {
		if (recursive) {
			for (const auto &dirEntry : std::filesystem::recursive_directory_iterator(dir)) {
				entries.push_back(FileEntry{
					.path = Utils::Format("{}://{}", _scheme, std::filesystem::relative(dirEntry.path(), _basePath).string()),
					.is_directory = dirEntry.is_directory()});
			}
		} else {
			for (const auto &dirEntry : std::filesystem::directory_iterator(dir)) {
				entries.push_back(FileEntry{
					.path = Utils::Format("{}://{}", _scheme, std::filesystem::relative(dirEntry.path(), _basePath).string()),
					.is_directory = dirEntry.is_directory()});
			}
		}
	}

	std::sort(entries.begin(), entries.end());
	return entries;
}

void FolderFileServer::GetSaveStream(const std::filesystem::path &path, std::ofstream &out) {
	out = std::ofstream(GetPath(_fs->ResolvePath(path).path));
}

bool FolderFileServer::IsDirectory(const std::filesystem::path &path) {
	try {
		return std::filesystem::is_directory(GetPath(_fs->ResolvePath(path).path));
	} catch (...) {
		return false;
	}
}

bool FolderFileServer::Remove(const std::filesystem::path &path) {
	try {
		return std::filesystem::remove(GetPath(_fs->ResolvePath(path).path));
	} catch (...) {
		return false;
	}
}

uintmax_t FolderFileServer::RemoveAll(const std::filesystem::path &path) {
	try {
		return std::filesystem::remove_all(GetPath(_fs->ResolvePath(path).path));
	} catch (...) {
		return false;
	}
}

bool FolderFileServer::Rename(const std::filesystem::path &p, const std::string &name) {
	try {
		std::filesystem::path path = GetPath(_fs->ResolvePath(p).path);
		std::filesystem::rename(path, path.parent_path().concat("/").concat(name));
		return true;
	} catch (...) {
		return false;
	}
}

bool FolderFileServer::Create(const std::filesystem::path &path) {
	try {
		std::ofstream out(GetPath(_fs->ResolvePath(path).path), std::ios::binary);
		return out.good();
	} catch (...) {
		return false;
	}
}

bool FolderFileServer::Exists(const std::filesystem::path &path) {
	try {
		return std::filesystem::exists(GetPath(_fs->ResolvePath(path).path));
	} catch (...) {
		return false;
	}
}

std::filesystem::path FolderFileServer::GetPath(const std::filesystem::path &path) {
	return _basePath / path;
}