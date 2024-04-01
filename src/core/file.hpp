#ifndef FILE_HPP
#define FILE_HPP
#pragma once

#include <cstddef>
#include <memory>
#include <vector>

struct FileDataInternal {
	std::vector<std::byte> buffer;
};
using FileData = std::shared_ptr<FileDataInternal>;

namespace File {
void InitState();

FileData Load(const char *path);

} // namespace File

#endif // FILE_HPP