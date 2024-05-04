#ifndef RENDERER2D_HPP
#define RENDERER2D_HPP
#pragma once

#include <map>
#include <vector>

#include "data/color.hpp"

#include "gl.hpp"
#include "gl/buffer.hpp"
#include "gl/vertex_array.hpp"
#include "resource/image_texture.hpp"
#include "shader.hpp"

#include "resource/font.hpp"

struct DefaultVertex2D {
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	float u = 0.f;
	float v = 0.f;

	float r = 1.f;
	float g = 1.f;
	float b = 1.f;
	float a = 1.f;

	float d_id = 0.f; // draw id
	float t_id = 0.f; // texture id
};

class Renderer2D {
  public:
	void Init(const char *shaderPath);

	void Reset();
	void End();

	void PushQuad(DefaultVertex2D vertices[4]);
	void PushQuad(float x, float y, float z, float w, float h, float r, float g, float b, float a, float drawID, float textureID);
	void PushQuad(glm::mat4 transform, float textureID, glm::vec2 textureScale, float z = 0.f, Color color = Color{}, float drawID = 1.f);
	void DrawText(const std::string &text, Font &font, const glm::mat4 &transform, float z = 0.f, Color color = Color{});

  private:
	Shader _shader;

	Buffer _buffer;
	VertexArray _vao;
	std::vector<DefaultVertex2D> _vertices;

	// textures[textureID] = slot;
	std::map<uint32_t, uint32_t> _textures;
	uint32_t _textureCounter = 0;

	std::unique_ptr<ImageTexture> _blankTexture;
};

#endif // RENDERER2D_HPP