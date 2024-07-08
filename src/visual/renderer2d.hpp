#ifndef RENDERER2D_HPP
#define RENDERER2D_HPP
#pragma once

#include <map>
#include <vector>

#include "glm/glm.hpp"

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

struct PushQuadArgs {
	glm::mat4 transform = glm::mat4(1.f);
	float textureID = 0.f;
	float z = 0.f;
	float drawID = 1.f;
	Color color = Color{};
	glm::vec2 textureScale = glm::vec2(1.f, 1.f);
	glm::vec2 uvTopLeft = glm::vec2(0.f, 1.f);
	glm::vec2 uvBottomRight = glm::vec2(1.f, 0.f);
};

class Renderer2D {
  public:
	void Init(const char *vertexPath, const char *fragmentPath);

	void Reset();
	void End();

	void PushQuad(DefaultVertex2D vertices[4]);
	void PushQuad(float x, float y, float z, float w, float h, float r, float g, float b, float a, float drawID, float textureID);
	void PushQuad(glm::mat4 transform, float textureID, glm::vec2 textureScale, float z = 0.f, Color color = Color{}, float drawID = 1.f);
	void PushQuad(const PushQuadArgs &args);
	void DrawText(const std::string &text, Font &font, const glm::mat4 &transform, float z = 0.f, Color color = Color{});

	void DrawLine(const glm::vec2& p1, const glm::vec2& p2, float thickness, Color color = Color{});

	void SetProjectionMatrix(const glm::mat4 &proj);

  private:
	Shader _shader;
	glm::mat4 _projection;

	Buffer _buffer;
	VertexArray _vao;
	std::vector<DefaultVertex2D> _vertices;

	// textures[textureID] = slot;
	std::map<uint32_t, uint32_t> _textures;
	uint32_t _textureCounter = 0;

	std::unique_ptr<ImageTexture> _blankTexture;
};

#endif // RENDERER2D_HPP