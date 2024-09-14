#ifndef ZIP_READER_HPP
#define ZIP_READER_HPP
#pragma once

#include <string>
#include <vector>

#include "sowa.hpp"

class FileData;

class ZipReader {
  public:
	// ZipReader();
	~ZipReader();
	void Cleanup();

	void OpenFile(const char *path);

	Ref<FileData> ReadFirstEntry();
	Ref<FileData> ReadNextEntry();
	Ref<FileData> ReadEntry(const char *filename);

	bool OpenFirstEntry();
	bool OpenNextEntry();
	bool OpenEntry(const char *filename);

	std::string CurrentEntryFilename();
	Ref<FileData> ReadCurrentEntry();
	void CloseCurrentEntry();

	std::vector<std::string> GetAllFilenames(const std::string &directory = "", bool recursive = true);

  private:
	void *_zipReader = nullptr;
	void *_memStream = nullptr;
	Ref<FileData> _file = nullptr;
};

#endif // ZIP_READER_HPP