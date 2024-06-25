#include "renderer.hpp"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gl/buffer.hpp"
#include "gl/vertex_array.hpp"
#include "model.hpp"
#include "model_builder.hpp"
#include "shader.hpp"

#include "resource/image_texture.hpp"

#include "utf8.h"

#ifndef PI
#define PI (3.14159265358979323846)
#endif

#define BATCH2D_MAX_RECT (1000)
#define BATCH2D_MAX_VERTEX (BATCH2D_MAX_RECT * 6)
#define BATCH2D_MAX_TEXTURE 16

void Renderer::Init() {
	ModelBuilder::Quad2D(_fullscreenModel, 2.f);

	_shader3d.Load("data://default3d.vs", "data://default3d.fs");
	_fullscreenShader.Load("data://fullscreen.vs", "data://fullscreen.fs");
}

void Renderer::RegisterRenderer2D(const char *name, const char *vertexPath, const char *fragmentPath) {
	Renderer2D &renderer = _renderer2ds[name];
	renderer.Init(vertexPath, fragmentPath);
}

Renderer2D &Renderer::GetRenderer2D(const char *name) {
	return _renderer2ds[name];
}

void Renderer::BeginDraw() {
	for (auto &[name, renderer] : _renderer2ds) {
		renderer.Reset();
	}
}

void Renderer::EndDraw() {
	for (auto &[name, renderer] : _renderer2ds) {
		renderer.End();
	}
}
void Renderer::DrawFullscreen(uint32_t textureID) {
	_fullscreenShader.Bind();
	_fullscreenShader.UniformTexture("uTexture", textureID, 0);
	_fullscreenModel.Draw();
}

void Renderer::DrawMesh(const Mesh &mesh, const glm::mat4 &transform) {
	_shader3d.Bind();

	_shader3d.UniformMat4("uView", glm::mat4(1.f));
	_shader3d.UniformMat4("uProj", glm::perspective(glm::radians(90.f), 16.f / 9.f, 0.1f, 100.f));
	_shader3d.UniformMat4("uModel", transform);
	_shader3d.UniformTexture("uMat_AlbedoTex", 3);

	mesh.Draw();
}