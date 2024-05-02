#ifndef IMAGE_TEXTURE_HPP
#define IMAGE_TEXTURE_HPP
#pragma once

#include <cstdint>

#include "core/resource.hpp"

class ImageTexture : public Resource {
  public:
	ImageTexture() = default;
	~ImageTexture();

	void Bind(int slot = 0);
	void Unbind();

	void Load(const char *path);
	// data must be RGBA
	void LoadFromData(unsigned char *data, int width, int height);

	void Delete();

	inline uint32_t ID() const { return _id; }
	inline int Width() const { return _width; }
	inline int Height() const { return _height; }
	inline int Channels() const { return _channels; }

  private:
	uint32_t _id = 0;
	int _width = 0;
	int _height = 0;
	int _channels = 0;
};

#endif // IMAGE_TEXTURE_HPP