#ifndef FONT_HPP
#define FONT_HPP
#pragma once

#include <glm/glm.hpp>
#include <map>
#include <string>

#include "core/filesystem/filesystem.hpp"
#include "core/resource.hpp"

class Font : public Resource {
  public:
	Font();
	virtual ~Font();

	void LoadTTF(const char *path);

	uint32_t GetGlyphTextureID(int charcode);
	glm::vec2 CalcTextSize(const std::string &text);

	struct Character {
		uint32_t textureID = 0;
		glm::ivec2 size;
		glm::ivec2 bearing;
		uint32_t advance;
	};

	inline const Character &GetCharacter(int charcode) { return _characters[charcode]; }

	void LoadCharacter(int charcode);

  private:
	void LoadFont();

	void *_face = nullptr;
	FileData _buffer;

	std::map<int, Font::Character> _characters;
};

#endif // FONT_HPP