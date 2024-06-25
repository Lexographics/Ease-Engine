#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP
#pragma once

#include <cstdint>
#include <map>

enum class ViewportTargetType {
	Vec4 = 0,
	Int,
};

struct ViewportTarget {
	ViewportTargetType type;
	uint32_t textureID;
};

class Viewport {
  public:
	Viewport();
	~Viewport();

	void Create(int width, int height);
	void Resize(int width, int height);
	void Delete();

	void SetTarget(uint32_t slot, ViewportTargetType type);

	void Bind();
	void Unbind();

	void Clear(float r, float g, float b, float a = 1.0, bool depth = false);
	int GetTargetTextureID(int slot);

	int ReadAttachmentInt(int slot, int x, int y);

	inline int Width() { return _width; }
	inline int Height() { return _height; }

  private:
	std::map<uint32_t, ViewportTarget> _targets;
	int _width = 0;
	int _height = 0;

	uint32_t _id = 0;
	uint32_t _rbo = 0;
};

#endif // VIEWPORT_HPP