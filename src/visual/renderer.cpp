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
	// _vao.New();
	// _buffer.New(BufferType::VertexBuffer);

	// _vao.Bind();
	// _buffer.Bind();

	// _buffer.BufferData(nullptr, sizeof(BatchVertex) * BATCH2D_MAX_VERTEX, BufferUsage::DynamicDraw);

	// _vao.ResetAttributes();
	// _vao.SetAttribute(0, AttributeType::Vec3);
	// _vao.SetAttribute(1, AttributeType::Vec2);
	// _vao.SetAttribute(2, AttributeType::Vec4);
	// _vao.SetAttribute(3, AttributeType::Float);
	// _vao.SetAttribute(4, AttributeType::Float);
	// _vao.UploadAttributes();

	// _vao.Unbind();

	_shader3d.Load("res/shaders/default3d");

	// _shader.Load("res/shaders/sprite2d");
	// _shader.Bind();

	// unsigned char pixel[] = {255, 255, 255, 255};
	// _blankTexture = std::make_unique<ImageTexture>();
	// _blankTexture->LoadFromData(pixel, 1, 1);

	ModelBuilder::Quad2D(_fullscreenModel, 2.f);
	_fullscreenShader.Load("res/shaders/fullscreen");
}

void Renderer::RegisterRenderer2D(const char *name, const char *shaderPath) {
	Renderer2D &renderer = _renderer2ds[name];
	renderer.Init(shaderPath);
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