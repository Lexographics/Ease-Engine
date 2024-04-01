#include "file.hpp"

#include <filesystem>
#include <fstream>

struct FileState {
};
static FileState state;

void File::InitState() {
}

FileData File::Load(const char *path) {
	FileData data = std::make_shared<FileDataInternal>();

	std::ifstream file{path, std::ios::binary};
	if (!file.good())
		return nullptr;

	file.seekg(0, std::ios::end);
	int size = static_cast<int>(file.tellg());
	file.seekg(0, std::ios::beg);

	data->buffer.resize(size);
	file.read(reinterpret_cast<char *>(data->buffer.data()), static_cast<long>(size));

	return data;
}