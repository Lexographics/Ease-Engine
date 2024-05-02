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

struct RendererState {
	Shader shader;
	Buffer buffer;
	VertexArray vao;
	std::vector<BatchVertex> vertices;

	// textures[textureID] = slot;
	std::map<uint32_t, uint32_t> textures;
	uint32_t textureCounter = 0;

	std::unique_ptr<ImageTexture> blankTexture;

	Model fullscreenModel;
	Shader fullscreenShader;

	Shader shader3d;
};
static RendererState state;

void Visual::Renderer::InitState() {
	state.vao.New();
	state.buffer.New(BufferType::VertexBuffer);

	state.vao.Bind();
	state.buffer.Bind();

	state.buffer.BufferData(nullptr, sizeof(BatchVertex) * BATCH2D_MAX_VERTEX, BufferUsage::DynamicDraw);

	state.vao.ResetAttributes();
	state.vao.SetAttribute(0, AttributeType::Vec3);
	state.vao.SetAttribute(1, AttributeType::Vec2);
	state.vao.SetAttribute(2, AttributeType::Vec4);
	state.vao.SetAttribute(3, AttributeType::Float);
	state.vao.SetAttribute(4, AttributeType::Float);
	state.vao.UploadAttributes();

	state.vao.Unbind();

	state.shader3d.Load("res/default3d");

	state.shader.Load("res/sprite2d");
	state.shader.Bind();

	unsigned char pixel[] = {255, 255, 255, 255};
	state.blankTexture = std::make_unique<ImageTexture>();
	state.blankTexture->LoadFromData(pixel, 1, 1);

	ModelBuilder::Quad2D(state.fullscreenModel, 2.f);
	state.fullscreenShader.Load("res/fullscreen");
}

void Visual::Renderer::Reset() {
	state.vertices.clear();
	state.textures.clear();
	state.textureCounter = 0;
}

static void PushQuad(BatchVertex vertices[4]) {
	// 1 2 3
	// 1 3 4
	for (int i = 0; i < 4; i++) {
		uint32_t textureID = static_cast<uint32_t>(vertices[i].t_id);
		if (textureID == 0) {
			textureID = state.blankTexture->ID();
		}

		if (state.textures[textureID] == 0) {
			state.textures[textureID] = ++state.textureCounter;
		}

		vertices[i].t_id = static_cast<float>(state.textures[textureID]) - 1.f;
	}

	state.vertices.push_back(vertices[0]);
	state.vertices.push_back(vertices[1]);
	state.vertices.push_back(vertices[2]);

	state.vertices.push_back(vertices[0]);
	state.vertices.push_back(vertices[2]);
	state.vertices.push_back(vertices[3]);

	if (state.textures.size() >= BATCH2D_MAX_TEXTURE || state.vertices.size() >= BATCH2D_MAX_VERTEX) {
		Visual::Renderer::End();
	}
}

void Visual::Renderer::PushQuad(float x, float y, float w, float h, float r, float g, float b, float a, float drawID, float textureID) {
	/*
		{ 0.0f, 1.0f,  0.f, 1.f}
		{ 0.0f, 0.0f,  0.f, 1.f}
		{ 1.0f, 0.0f,  0.f, 1.f}
		{ 1.0f, 1.0f,  0.f, 1.f}
		*/

	glm::vec4 points[4] = {
		{x, y + h, 0.f, 1.f},
		{x, y, 0.f, 1.f},
		{x + w, y, 0.f, 1.f},
		{x + w, y + h, 0.f, 1.f}};
	glm::vec2 uvs[4] = {
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		{1.f, 1.f}};

	BatchVertex vertices[4];
	for (int i = 0; i < 4; i++) {
		vertices[i].x = points[i].x;
		vertices[i].y = points[i].y;
		vertices[i].z = 0.f;
		vertices[i].r = r;
		vertices[i].g = g;
		vertices[i].b = b;
		vertices[i].a = a;
		vertices[i].u = uvs[i].x;
		vertices[i].v = uvs[i].y;
		vertices[i].t_id = textureID;
		vertices[i].d_id = drawID;
	}

	PushQuad(vertices);
}

void Visual::Renderer::PushQuad(glm::mat4 transform, float textureID, glm::vec2 textureScale, float r /*= 1.f*/, float g /*= 1.f*/, float b /*= 1.f*/, float a /*= 1.f*/, float drawID /*= 1.f*/) {
	glm::vec4 points[4] = {
		{-0.5f * textureScale.x, 0.5f * textureScale.y, 0.f, 1.f},
		{-0.5f * textureScale.x, -0.5f * textureScale.y, 0.f, 1.f},
		{0.5f * textureScale.x, -0.5f * textureScale.y, 0.f, 1.f},
		{0.5f * textureScale.x, 0.5f * textureScale.y, 0.f, 1.f}};
	glm::vec2 uvs[4] = {
		{0.f, 1.f},
		{0.f, 0.f},
		{1.f, 0.f},
		{1.f, 1.f}};

	BatchVertex vertices[4];
	for (int i = 0; i < 4; i++) {
		points[i] = transform * points[i];

		vertices[i].x = points[i].x;
		vertices[i].y = points[i].y;
		vertices[i].z = 0.f;
		vertices[i].r = r;
		vertices[i].g = g;
		vertices[i].b = b;
		vertices[i].a = a;
		vertices[i].u = uvs[i].x;
		vertices[i].v = uvs[i].y;
		vertices[i].t_id = textureID;
		vertices[i].d_id = drawID;
	}

	PushQuad(vertices);
}

void Visual::Renderer::End() {
	if (state.vertices.size() == 0)
		return;

	state.shader.Bind();
	state.buffer.Bind();
	state.buffer.BufferSubdata(state.vertices.data(), state.vertices.size() * sizeof(BatchVertex), 0);
	state.buffer.Unbind();

	std::vector<int> textures;
	for (const auto &[id, slot] : state.textures) {
		glActiveTexture(GL_TEXTURE0 + (slot - 1));
		glBindTexture(GL_TEXTURE_2D, id);
		textures.push_back(slot - 1);
	}

	std::sort(textures.begin(), textures.end());
	state.shader.Uniformiv("uTextures", textures);

	state.shader.UniformMat4("uView", glm::mat4(1.f));
	state.shader.UniformMat4("uProj", glm::ortho(0.f, 1280.f, 0.f, 720.f, -128.f, 128.f));

	glDisable(GL_DEPTH_TEST);
	state.vao.Bind();
	glDrawArrays(GL_TRIANGLES, 0, state.vertices.size());
	state.vao.Unbind();

	Reset();
}

void Visual::Renderer::DrawText(const std::string &text, Font &font, const glm::mat4 &transform) {
	float x = 0;
	float y = 0;

	auto it = text.begin();
	auto end = text.end();

	while (it != end) {
		utf8::utfchar32_t charcode = utf8::next(it, end);

		const Font::Character &ch = font.GetCharacter(charcode);
		if (ch.textureID == 0) {
			std::cout << "Texture id for character " << charcode << " not found" << std::endl;
			font.LoadCharacter(charcode);
			continue;
		}

		float xPos = x + ch.bearing.x;
		float yPos = y - (ch.size.y - ch.bearing.y);

		float w = ch.size.x;
		float h = ch.size.y;

		glm::vec4 points[4] = {
			{xPos, yPos + h, 0.f, 1.f},
			{xPos, yPos, 0.f, 1.f},
			{xPos + w, yPos, 0.f, 1.f},
			{xPos + w, yPos + h, 0.f, 1.f}};

		glm::vec2 uvs[4] = {
			{0.f, 0.f},
			{0.f, 1.f},
			{1.f, 1.f},
			{1.f, 0.f}};

		BatchVertex vertices[4];
		for (int i = 0; i < 4; i++) {
			points[i] = transform * points[i];

			vertices[i].x = points[i].x;
			vertices[i].y = points[i].y;
			vertices[i].z = 1.f;
			vertices[i].r = 1.f;
			vertices[i].g = 1.f;
			vertices[i].b = 1.f;
			vertices[i].a = 1.f;
			vertices[i].t_id = ch.textureID;
			vertices[i].d_id = 1.f;
			vertices[i].u = uvs[i].x;
			vertices[i].v = uvs[i].y;
		}

		PushQuad(vertices);
		x += (ch.advance >> 6);
	}
}

void Visual::Renderer::DrawFullscreen(uint32_t textureID) {
	state.fullscreenShader.Bind();
	state.fullscreenShader.UniformTexture("uTexture", textureID, 0);
	state.fullscreenModel.Draw();
}

void Visual::Renderer::DrawMesh(const Mesh &mesh, const glm::mat4 &transform) {
	state.shader3d.Bind();

	state.shader3d.UniformMat4("uView", glm::mat4(1.f));
	state.shader3d.UniformMat4("uProj", glm::perspective(glm::radians(90.f), 16.f / 9.f, 0.1f, 100.f));
	state.shader3d.UniformMat4("uModel", transform);
	state.shader3d.UniformTexture("uMat_AlbedoTex", 2);

	mesh.Draw();
}