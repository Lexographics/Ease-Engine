#ifndef _E_FILE_HPP__
#define _E_FILE_HPP__
#pragma once

#include "stlpch.hpp"
#include "core/file_buffer.hpp"

namespace sowa {
namespace File {

struct FilePathData {
	std::filesystem::path relativePath;
	std::filesystem::path endpoint;
};
/**
 * @brief Returns real path from given string like res://path/file -> ./ProjectPath/path/file
 *
 * @param path path with delimiter (like res://)
 * @param pathdata fill information about path if given
 * @return std::string real path
 */
std::filesystem::path Path(const std::filesystem::path &path, FilePathData *pathdata = nullptr);

/**
 * @brief Add Custom Endpoint
 *
 * @param path should be a directory
 * @param force modify endpoint if it already exists
 * @return true if endpoint inserted
 * @return false if endpoint already exists
 */
bool InsertFilepathEndpoint(const std::string &endpoint, const std::filesystem::path &path, bool force = false);

/**
 * @brief Registers data:// and usr://, editor:// endpoints.
 *
 * @return data path found and registered
 */
bool RegisterDataPath();

/**
 * @brief Return file content and size of given file path
 *
 * @param path with delimiter! (res://image.png ...)
 * @return std::vector<unsigned char> file content
 */
std::vector<unsigned char> GetFileContent(const char *path);

FileBuffer LoadFile(std::filesystem::path path);

/**
 * @brief Creates temp file with given data
 *
 * @param data
 * @param size
 * @return std::filesystem::path of the file
 */
std::filesystem::path CreateTempFile(unsigned char *data, size_t size);
} // namespace File
} // namespace sowa
#endif // _E_FILE_HPP__