#ifndef RENDERER_HPP
#define RENDERER_HPP
#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "model.hpp"
#include "renderer2d.hpp"
#include "shader.hpp"

#include "resource/font.hpp"
#include "resource/image_texture.hpp"
#include "resource/mesh.hpp"

#include "data/color.hpp"

struct BatchVertex {
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

class Renderer {
  public:
	void Init();

	void RegisterRenderer2D(const char *name, const char *shaderPath);
	Renderer2D &GetRenderer2D(const char *name);

	void BeginDraw();
	void EndDraw();

	void DrawFullscreen(uint32_t textureID);

	void DrawMesh(const Mesh &mesh, const glm::mat4 &transform);

  private:
	Model _fullscreenModel;
	Shader _fullscreenShader;

	Shader _shader3d;

	std::unordered_map<std::string, Renderer2D> _renderer2ds;
};

#endif // RENDERER_HPP