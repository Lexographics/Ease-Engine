#include "zip_file_server.hpp"

#include "core/filesystem/filesystem.hpp"
#include "utils/utils.hpp"

ZipFileServer::ZipFileServer(const char *scheme, const std::filesystem::path &path) {
	_scheme = scheme;
	_reader.OpenFile(path.c_str());
}

Ref<FileData> ZipFileServer::Load(const std::filesystem::path &path) {
	return _reader.ReadEntry(path.c_str());
}

std::vector<FileEntry> ZipFileServer::ReadDirectory(const std::filesystem::path &path, bool recursive) {
	auto files = _reader.GetAllFilenames(path, recursive);
	std::vector<FileEntry> entries;
	for (const auto &file : files) {
		FileEntry entry;
		entries.push_back(FileEntry{
			.path = Utils::Format("{}://{}", _scheme, path.string()),
			.is_directory = false});
	}

	return entries;
}