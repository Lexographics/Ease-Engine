#ifndef ZIP_WRITER_HPP
#define ZIP_WRITER_HPP
#pragma once

#include "core/filesystem/filesystem.hpp"

class ZipWriter {
  public:
	ZipWriter();
	~ZipWriter();

	void AddFile(Ref<FileData> buffer, const char *filename);
	void AddFile(const char *srcFile, const char *filename); // srcFile should be schemed path

	void Create();	// Automatically called at construction
	void Cleanup(); // Automatically called at destruction

	Ref<FileData> SaveToBuffer(); // Calls Cleanup

  private:
	void *_memStream = nullptr;
	void *_zipWriter = nullptr;
};

#endif // ZIP_WRITER_HPP