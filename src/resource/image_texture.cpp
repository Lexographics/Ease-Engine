#include "image_texture.hpp"

#include <iostream>

#include "visual/gl.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "core/application.hpp"
#include "core/debug.hpp"

ImageTexture::ImageTexture() {
	_resourceType = typeid(ImageTexture).hash_code();
}

ImageTexture::~ImageTexture() {
	Delete();
}

void ImageTexture::Bind(int slot /*= 0*/) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, _id);
}

void ImageTexture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ImageTexture::Load(const char *path) {
	Delete();

	auto file = App().FS().Load(path);
	if (!file) {
		return;
	}

	stbi_set_flip_vertically_on_load(!_flip);
	auto pixels = stbi_load_from_memory(reinterpret_cast<stbi_uc *>(file->Data()), file->Size(), &_width, &_height, &_channels, 4);
	if (!pixels) {
		Delete();
		std::cout << "Failed to load texture path:" << path << std::endl;
		return;
	}

	_filepath = path;
	LoadFromData(pixels, _width, _height);

	if (_storePixels) {
		_pixels = pixels;
	} else {
		stbi_image_free(pixels);
	}

#ifdef SW_EDITOR
	if (_watchID == 0) {
		_watchID = App().GetEditor().GetHotReloader().WatchPath(path, [this](const std::string &path, HotReloader::EventType eventType) {
			if (eventType == HotReloader::EventType::Updated) {
				Load(path.c_str());
				Debug::Log("Reloaded texture: {}", path);
			}
		});
	}
#endif
}

void ImageTexture::LoadFromData(unsigned char *data, int width, int height) {
	Delete();

	_width = width;
	_height = height;
	_channels = 4;

	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void ImageTexture::Delete() {
	if (_pixels != nullptr) {
		stbi_image_free(_pixels);
		_pixels = nullptr;
	}

	if (_id != 0) {
		glDeleteTextures(1, &_id);
		_id = 0;
	}

#ifdef SW_EDITOR
	if (_watchID != 0) {
		App().GetEditor().GetHotReloader().RemoveWatcher(_watchID);
		_watchID = 0;
	}
#endif
}