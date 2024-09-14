#include "zip_reader.hpp"

#include "mz.h"
#include "mz_os.h"
#include "mz_strm.h"
#include "mz_strm_mem.h"
#include "mz_strm_os.h"
#include "mz_zip.h"
#include "mz_zip_rw.h"
#include "unzip.h"
#include "zip.h"

#include "core/application.hpp"
#include "core/debug.hpp"

#include "core/filesystem/filesystem.hpp"

ZipReader::~ZipReader() {
	Cleanup();
}

void ZipReader::Cleanup() {
	if (_zipReader != nullptr) {
		mz_zip_reader_close(_zipReader);
		mz_zip_reader_delete(&_zipReader);
	}

	if (_memStream != nullptr) {
		mz_stream_mem_close(_memStream);
		mz_stream_mem_delete(&_memStream);
	}
}

void ZipReader::OpenFile(const char *path) {
	Cleanup();

	_file = App().FS().Load(path);
	if (!_file) {
		Debug::Error("Failed to open zip file: {}", path);
		return;
	}

	_zipReader = mz_zip_reader_create();
	_memStream = mz_stream_mem_create();

	mz_stream_mem_set_buffer(_memStream, (void *)_file->Data(), _file->Size());
	mz_zip_reader_open(_zipReader, _memStream);
}

Ref<FileData> ZipReader::ReadFirstEntry() {
	if (!OpenFirstEntry())
		return nullptr;
	Ref<FileData> file = ReadCurrentEntry();
	CloseCurrentEntry();
	return file;
}
Ref<FileData> ZipReader::ReadNextEntry() {
	if (!OpenNextEntry())
		return nullptr;
	Ref<FileData> file = ReadCurrentEntry();
	CloseCurrentEntry();
	return file;
}
Ref<FileData> ZipReader::ReadEntry(const char *filename) {
	if (!OpenEntry(filename))
		return nullptr;
	Ref<FileData> file = ReadCurrentEntry();
	CloseCurrentEntry();
	return file;
}

bool ZipReader::OpenFirstEntry() {
	if (mz_zip_reader_goto_first_entry(_zipReader) != MZ_OK)
		return false;

	if (mz_zip_reader_entry_open(_zipReader) != MZ_OK)
		return false;

	return true;
}

bool ZipReader::OpenNextEntry() {
	if (mz_zip_reader_goto_next_entry(_zipReader) != MZ_OK)
		return false;

	if (mz_zip_reader_entry_open(_zipReader) != MZ_OK)
		return false;

	return true;
}

bool ZipReader::OpenEntry(const char *filename) {
	if (mz_zip_reader_locate_entry(_zipReader, filename, 0) != MZ_OK)
		return false;

	if (mz_zip_reader_entry_open(_zipReader) != MZ_OK)
		return false;

	return true;
}

std::string ZipReader::CurrentEntryFilename() {
	mz_zip_file *fileInfo = nullptr;
	mz_zip_reader_entry_get_info(_zipReader, &fileInfo);
	if (!fileInfo)
		return "";

	return fileInfo->filename;
}

Ref<FileData> ZipReader::ReadCurrentEntry() {
	Ref<FileData> file = FileData::New();

	mz_zip_file *fileInfo = nullptr;
	if (mz_zip_reader_entry_get_info(_zipReader, &fileInfo) != MZ_OK)
		return nullptr;

	file->Buffer().resize(fileInfo->uncompressed_size);

	char buf[1024];
	int32_t bytesRead = 0;
	auto cur = file->Buffer().begin();
	do {
		bytesRead = mz_zip_reader_entry_read(_zipReader, buf, sizeof(buf));
		memcpy((void *)&*cur, buf, bytesRead);
		cur += bytesRead;

	} while (bytesRead > 0);

	return file;
}

void ZipReader::CloseCurrentEntry() {
	mz_zip_reader_entry_close(_zipReader);
}

std::vector<std::string> ZipReader::GetAllFilenames(const std::string &directory, bool recursive) {
	std::vector<std::string> files;

	if (!OpenFirstEntry())
		return files;

	bool first = true;
	while (true) {
		if (!first && !OpenNextEntry()) {
			return files;
		}
		first = false;

		if (mz_zip_reader_entry_is_dir(_zipReader) == MZ_OK) {
			continue;
		}

		std::filesystem::path path(CurrentEntryFilename());
		std::string relative = std::filesystem::relative(path, directory).string();
		if (relative.size() >= 3 && relative[0] == '.' && relative[1] == '.' && (relative[2] == '/' || relative[2] == '\\')) {
			// if true, current entry is not in directory parameter
			continue;
		}

		if (!recursive && relative.find_first_of('/') != std::string::npos) {
			continue;
		}

		files.push_back(path.string());
	}

	return files;
}