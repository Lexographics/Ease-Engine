#include "font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "visual/gl.hpp"

#include "core/application.hpp"

static FT_Library GetFreeType() {
	static FT_Library lib = nullptr;
	if (!lib)
		FT_Init_FreeType(&lib);
	return lib;
}

Font::~Font() {
	FT_Done_Face(reinterpret_cast<FT_Face>(_face));
}

void Font::LoadTTF(const char *path) {
	_buffer = App().FS()->Load(path);
	if (!_buffer) {
		return;
	}

	LoadFont();
}

uint32_t Font::GetGlyphTextureID(int charcode) {
	return _characters[charcode].textureID;
}

glm::vec2 Font::CalcTextSize(const std::string &text) {
	glm::vec2 size{0.f, 0.f};

	float scale = 1.f;
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		auto ch = _characters[*c];
		size.x += (ch.advance >> 6) * scale;

		if (ch.size.y > size.y) {
			size.y = ch.size.y;
		}
	}
	return size;
}

void Font::LoadFont() {
	FT_Library freetype = GetFreeType();
	if (FT_New_Memory_Face(freetype, (const unsigned char *)_buffer->buffer.data(), _buffer->buffer.size(), 0, reinterpret_cast<FT_Face *>(&_face))) {
		return;
	}
	FT_Set_Pixel_Sizes(reinterpret_cast<FT_Face>(_face), 0, 48);

	if (FT_Load_Char(reinterpret_cast<FT_Face>(_face), 'X', FT_LOAD_RENDER)) {
		return;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (int c = 0; c < 128; c++) {
		LoadCharacter(c);
	}
}

void Font::LoadCharacter(int charcode) {
	FT_Face face = reinterpret_cast<FT_Face>(_face);
	FT_UInt glyphIndex = FT_Get_Char_Index(face, charcode);

	if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT)) {
		return;
	}

	if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL)) {
		return;
	}

	uint32_t tex = 0;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (face->glyph->bitmap.width > 0 && face->glyph->bitmap.rows > 0)
		glGenerateMipmap(GL_TEXTURE_2D);

	Font::Character ch;
	ch.textureID = tex;
	ch.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
	ch.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
	ch.advance = static_cast<uint32_t>(face->glyph->advance.x);

	_characters[charcode] = ch;
}