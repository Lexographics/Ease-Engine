#include "zip_writer.hpp"

#include <fstream>

#include "core/application.hpp"
#include "core/debug.hpp"

#include "mz.h"
#include "mz_os.h"
#include "mz_strm.h"
#include "mz_strm_mem.h"
#include "mz_strm_os.h"
#include "mz_zip.h"
#include "mz_zip_rw.h"
#include "unzip.h"
#include "zip.h"

// #define DO(f)
// 	if (f != MZ_OK)
// 	Debug::Error("Failed: {}", #f)

#define DO(f)                                                \
	do {                                                     \
		int err = f;                                         \
		if (err != MZ_OK)                                    \
			Debug::Error("Failed. Code: {} at {}", err, #f); \
	} while (false)

ZipWriter::ZipWriter() {
	Create();

	/*
	void *mem_stream = mz_stream_mem_create();
	DO(mz_stream_mem_open(mem_stream, NULL, MZ_OPEN_MODE_WRITE | MZ_OPEN_MODE_CREATE));

	void *zip_writer = mz_zip_writer_create();
	// void *file_stream = mz_stream_os_create();
	// DO(mz_stream_os_open(file_stream, "test.zip", MZ_OPEN_MODE_WRITE | MZ_OPEN_MODE_CREATE));
	// DO(mz_zip_writer_open(zip_writer, file_stream, 0));
	DO(mz_zip_writer_open(zip_writer, mem_stream, 0));

	mz_zip_file file_info;
	memset(&file_info, 0, sizeof(mz_zip_file));

	file_info.filename = "newfile.txt";
	file_info.modified_date = time(NULL);
	file_info.version_madeby = MZ_VERSION_MADEBY;
	file_info.compression_method = MZ_COMPRESS_METHOD_STORE;
	file_info.flag = MZ_ZIP_FLAG_UTF8;

	DO(mz_zip_writer_entry_open(zip_writer, &file_info));
	std::string a = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	mz_zip_writer_entry_write(zip_writer, (void *)a.c_str(), a.size());
	DO(mz_zip_writer_entry_close(zip_writer));

	mz_zip_writer_close(zip_writer);
	// mz_zip_writer_delete(&zip_writer);

	const void *buf;
	int32_t len;
	mz_stream_mem_get_buffer(mem_stream, &buf);
	mz_stream_mem_get_buffer_length(mem_stream, &len);

	Debug::Log("Buffer: {}, Size: {}", buf, len);
	auto fs = dynamic_cast<SaveableFileServer *>(App().FS().GetFileServer("res"));
	std::ofstream out;
	fs->GetSaveStream("res://test.zip", out);

	out.write((const char *)buf, len);
	out.close();

	// mz_stream_os_delete(&file_stream);
	mz_stream_mem_delete(&mem_stream);
	mz_zip_writer_delete(&zip_writer);
*/
}

void ZipWriter::AddFile(Ref<FileData> buffer, const char *filename) {
	if (!_zipWriter || !_memStream) {
		Debug::Error("Invalid zip writer");
		return;
	}
	if (!buffer) {
		Debug::Error("Can not AddFile, Invalid buffer");
		return;
	}

	mz_zip_file fileInfo;
	memset(&fileInfo, 0, sizeof(mz_zip_file));

	fileInfo.filename = filename;
	fileInfo.modified_date = time(NULL);
	fileInfo.version_madeby = MZ_VERSION_MADEBY;
	fileInfo.compression_method = MZ_COMPRESS_METHOD_STORE;
	fileInfo.flag = MZ_ZIP_FLAG_UTF8;
	if (mz_zip_writer_entry_open(_zipWriter, &fileInfo) != MZ_OK) {
		Debug::Error("Failed to open zip writer entry for {}", filename);
	}

	int32_t written = mz_zip_writer_entry_write(_zipWriter, (void *)buffer->Data(), buffer->Size());
	if (written != static_cast<int32_t>(buffer->Size())) {
		Debug::Error("Failed to write {} bytes to {}", buffer->Size(), filename);
	}
	if (mz_zip_writer_entry_close(_zipWriter) != MZ_OK) {
		Debug::Error("Faield to close zip writer entry");
	}
}

void ZipWriter::AddFile(const char *srcFile, const char *filename) {
	if (!_zipWriter || !_memStream) {
		Debug::Error("Invalid zip writer");
		return;
	}

	Ref<FileData> file = App().FS().Load(srcFile);
	AddFile(file, filename);
}

ZipWriter::~ZipWriter() {
	Cleanup();
}

void ZipWriter::Create() {
	Cleanup();

	_memStream = mz_stream_mem_create();
	if (mz_stream_mem_open(_memStream, NULL, MZ_OPEN_MODE_WRITE | MZ_OPEN_MODE_CREATE) != MZ_OK) {
		Debug::Error("Failed to open memory stream");
	}

	_zipWriter = mz_zip_writer_create();
	if (mz_zip_writer_open(_zipWriter, _memStream, 0) != MZ_OK) {
		Debug::Error("Failed to open zip writer");
	}
}

void ZipWriter::Cleanup() {
	if (_zipWriter != nullptr) {
		mz_zip_writer_close(_zipWriter);
		mz_zip_writer_delete(&_zipWriter);
	}

	if (_memStream != nullptr) {
		mz_stream_mem_close(_memStream);
		mz_stream_mem_delete(&_memStream);
	}
}

Ref<FileData> ZipWriter::SaveToBuffer() {
	if (!_zipWriter || !_memStream) {
		Debug::Error("Invalid zip writer");
		return nullptr;
	}

	mz_zip_writer_close(_zipWriter);

	const void *buf;
	int32_t len;
	mz_stream_mem_get_buffer(_memStream, &buf);
	mz_stream_mem_get_buffer_length(_memStream, &len);

	Ref<FileData> file = FileData::New();
	file->Buffer().resize(len);
	memcpy(file->Buffer().data(), buf, len);

	Cleanup();
	return file;
}