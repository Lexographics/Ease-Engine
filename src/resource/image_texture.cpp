#include "image_texture.hpp"

#include <iostream>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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
	stbi_set_flip_vertically_on_load(true);
	auto pixels = stbi_load(path, &_width, &_height, &_channels, 4);
	if (!pixels) {
		Delete();
		std::cout << "Failed to load texture path:" << path << std::endl;
		return;
	}

	LoadFromData(pixels, _width, _height);

	stbi_image_free(pixels);
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
}