#include "filesystem.hpp"

#include <filesystem>
#include <fstream>

#include "core/debug.hpp"

void FileSystem::RegisterFileServer(const char *scheme, FileServer *server) {
	_fileServers[scheme] = server;
}

FileServer *FileSystem::GetFileServer(const char *scheme) {
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

FileData FileSystem::Load(const std::filesystem::path &path) {
	PathData data = ResolvePath(path);
	if (!HasFileServer(data.scheme.c_str()) || _fileServers[data.scheme] == nullptr)
		return nullptr;

	return _fileServers[data.scheme]->Load(data.path);
}

std::vector<FileEntry> FileSystem::ReadDirectory(const std::filesystem::path &path) {
	PathData data = ResolvePath(path);
	if (!HasFileServer(data.scheme.c_str()) || _fileServers[data.scheme] == nullptr)
		return std::vector<FileEntry>();

	return _fileServers[data.scheme]->ReadDirectory(data.path);
}

FileServer *FileSystem::NewFolderFileServer(const char *scheme, const std::filesystem::path &path) {
	class FolderFileServer : public FileServer, public SaveableFileServer {
	  public:
		FolderFileServer(FileSystem *fs, const char *scheme, const std::filesystem::path &path) {
			_fs = fs;
			_scheme = scheme;
			_basePath = path;
		}

		FileData Load(const std::filesystem::path &path) {
			FileData data = std::make_shared<FileDataInternal>();

			std::ifstream file{GetPath(path), std::ios::binary};
			if (!file.good())
				return nullptr;

			file.seekg(0, std::ios::end);
			int size = static_cast<int>(file.tellg());
			file.seekg(0, std::ios::beg);

			data->buffer.resize(size);
			file.read(reinterpret_cast<char *>(data->buffer.data()), static_cast<long>(size));

			return data;
		}

		std::vector<FileEntry> ReadDirectory(const std::filesystem::path &path) {
			std::vector<FileEntry> entries;
			std::filesystem::path dir = GetPath(path);

			if (std::filesystem::is_directory(dir))
				for (const auto &dirEntry : std::filesystem::directory_iterator(dir)) {
					entries.push_back(FileEntry{
						.path = Utils::Format("{}://{}", _scheme, std::filesystem::relative(dirEntry.path(), _basePath).string()),
						.is_directory = dirEntry.is_directory()});
				}

			std::sort(entries.begin(), entries.end());
			return entries;
		}

		std::filesystem::path GetPath(const std::filesystem::path &path) {
			return _basePath / path;
		}

		void GetSaveStream(const std::filesystem::path &path, std::ofstream &out) {
			out = std::ofstream(GetPath(_fs->ResolvePath(path).path));
		}

	  private:
		FileSystem *_fs = nullptr;
		std::string _scheme = "";
		std::filesystem::path _basePath = "";
	};

	return reinterpret_cast<FileServer *>(new FolderFileServer(this, scheme, path));
}