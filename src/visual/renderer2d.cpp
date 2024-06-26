#include "renderer2d.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <utf8.h>

#include "gl/vertex_array.hpp"

#define MAX_VERTEX(max_quad) (max_quad * 6)

#define BATCH2D_MAX_RECT (1000)
#define BATCH2D_MAX_VERTEX (BATCH2D_MAX_RECT * 6)
#define BATCH2D_MAX_TEXTURE 16

void Renderer2D::Init(const char *vertexPath, const char *fragmentPath) {
	_projection = glm::ortho(0.f, 1280.f, 0.f, 720.f, -128.f, 128.f);

	_vao.New();
	_buffer.New(BufferType::VertexBuffer);

	_vao.Bind();
	_buffer.Bind();

	_buffer.BufferData(nullptr, sizeof(DefaultVertex2D) * BATCH2D_MAX_VERTEX, BufferUsage::DynamicDraw);

	_vao.ResetAttributes();
	_vao.SetAttribute(0, AttributeType::Vec3);
	_vao.SetAttribute(1, AttributeType::Vec2);
	_vao.SetAttribute(2, AttributeType::Vec4);
	_vao.SetAttribute(3, AttributeType::Float);
	_vao.SetAttribute(4, AttributeType::Float);
	_vao.UploadAttributes();
	_vao.Unbind();

	_shader.Load(vertexPath, fragmentPath);

	unsigned char pixel[] = {255, 255, 255, 255};
	_blankTexture = std::make_unique<ImageTexture>();
	_blankTexture->LoadFromData(pixel, 1, 1);
}

void Renderer2D::Reset() {
	_vertices.clear();
	_textures.clear();
	_textureCounter = 0;
}

void Renderer2D::PushQuad(DefaultVertex2D vertices[4]) {
	// 1 2 3
	// 1 3 4
	for (int i = 0; i < 4; i++) {
		uint32_t textureID = static_cast<uint32_t>(vertices[i].t_id);
		if (textureID == 0) {
			textureID = _blankTexture->ID();
		}

		if (_textures[textureID] == 0) {
			_textures[textureID] = ++_textureCounter;
		}

		vertices[i].t_id = static_cast<float>(_textures[textureID]) - 1.f;
	}

	_vertices.push_back(vertices[0]);
	_vertices.push_back(vertices[1]);
	_vertices.push_back(vertices[2]);

	_vertices.push_back(vertices[0]);
	_vertices.push_back(vertices[2]);
	_vertices.push_back(vertices[3]);

	if (_textures.size() >= BATCH2D_MAX_TEXTURE || _vertices.size() >= BATCH2D_MAX_VERTEX) {
		End();
	}
}

void Renderer2D::PushQuad(float x, float y, float z, float w, float h, float r, float g, float b, float a, float drawID, float textureID) {
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

	DefaultVertex2D vertices[4];
	for (int i = 0; i < 4; i++) {
		vertices[i].x = points[i].x;
		vertices[i].y = points[i].y;
		vertices[i].z = z;
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

void Renderer2D::PushQuad(glm::mat4 transform, float textureID, glm::vec2 textureScale, float zIndex, Color color, float drawID) {
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

	DefaultVertex2D vertices[4];
	for (int i = 0; i < 4; i++) {
		points[i] = transform * points[i];

		vertices[i].x = points[i].x;
		vertices[i].y = points[i].y;
		vertices[i].z = zIndex;
		vertices[i].r = color.r;
		vertices[i].g = color.g;
		vertices[i].b = color.b;
		vertices[i].a = color.a;
		vertices[i].u = uvs[i].x;
		vertices[i].v = uvs[i].y;
		vertices[i].t_id = textureID;
		vertices[i].d_id = drawID;
	}

	PushQuad(vertices);
}

void Renderer2D::PushQuad(const PushQuadArgs &args) {
	glm::vec4 points[4] = {
		{-0.5f * args.textureScale.x, 0.5f * args.textureScale.y, 0.f, 1.f},
		{-0.5f * args.textureScale.x, -0.5f * args.textureScale.y, 0.f, 1.f},
		{0.5f * args.textureScale.x, -0.5f * args.textureScale.y, 0.f, 1.f},
		{0.5f * args.textureScale.x, 0.5f * args.textureScale.y, 0.f, 1.f}};

	glm::vec2 uvs[4] = {
		{args.uvTopLeft.x, args.uvTopLeft.y},
		{args.uvTopLeft.x, args.uvBottomRight.y},
		{args.uvBottomRight.x, args.uvBottomRight.y},
		{args.uvBottomRight.x, args.uvTopLeft.y}};

	DefaultVertex2D vertices[4];
	for (int i = 0; i < 4; i++) {
		points[i] = args.transform * points[i];

		vertices[i].x = points[i].x;
		vertices[i].y = points[i].y;
		vertices[i].z = args.z;
		vertices[i].r = args.color.r;
		vertices[i].g = args.color.g;
		vertices[i].b = args.color.b;
		vertices[i].a = args.color.a;
		vertices[i].u = uvs[i].x;
		vertices[i].v = uvs[i].y;
		vertices[i].t_id = args.textureID;
		vertices[i].d_id = args.drawID;
	}

	PushQuad(vertices);
}

void Renderer2D::End() {
	if (_vertices.size() == 0)
		return;

	_shader.Bind();
	_buffer.Bind();
	_buffer.BufferSubdata(_vertices.data(), _vertices.size() * sizeof(DefaultVertex2D), 0);
	_buffer.Unbind();

	std::vector<int> textures;
	for (const auto &[id, slot] : _textures) {
		glActiveTexture(GL_TEXTURE0 + (slot - 1));
		glBindTexture(GL_TEXTURE_2D, id);
		textures.push_back(slot - 1);
	}

	std::sort(textures.begin(), textures.end());
	_shader.Uniformiv("uTextures", textures);

	_shader.UniformMat4("uView", glm::mat4(1.f));
	_shader.UniformMat4("uProj", _projection);

	glEnable(GL_DEPTH_TEST);
	_vao.Bind();
	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
	_vao.Unbind();
	glDisable(GL_DEPTH_TEST);

	Reset();
}

void Renderer2D::DrawText(const std::string &text, Font &font, const glm::mat4 &transform, float z, Color color) {
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

		DefaultVertex2D vertices[4];
		for (int i = 0; i < 4; i++) {
			points[i] = transform * points[i];

			vertices[i].x = points[i].x;
			vertices[i].y = points[i].y;
			vertices[i].z = z;
			vertices[i].r = color.r;
			vertices[i].g = color.g;
			vertices[i].b = color.b;
			vertices[i].a = color.a;
			vertices[i].t_id = ch.textureID;
			vertices[i].d_id = 1.f;
			vertices[i].u = uvs[i].x;
			vertices[i].v = uvs[i].y;
		}

		PushQuad(vertices);
		x += (ch.advance >> 6);
	}
}

void Renderer2D::SetProjectionMatrix(const glm::mat4 &proj) {
	_projection = proj;
}

/*

void Renderer2D::Init(const char *shaderPath, int maxQuad, int maxTexture) {
	_maxQuad = maxQuad;
	_maxTexture = maxTexture;

	_shader.Load(shaderPath);
	_shader.Bind();

	_vao.New();
	_buffer.New(BufferType::VertexBuffer);

	_vao.ResetAttributes();

	////
	GLint size;
	GLenum type;

	int totalSize = 0;

	const GLsizei bufSize = 16;
	GLchar name[bufSize];
	GLsizei length;

	GLint count;
	glGetProgramiv(_shader.ID(), GL_ACTIVE_ATTRIBUTES, &count);
	for (GLuint i = 0; i < static_cast<GLuint>(count); i++) {
		glGetActiveAttrib(_shader.ID(), i, bufSize, &length, &size, &type, name);
		printf("Attribute #%d Type: %u Name: %s\n", i, type, name);

		AttributeType attrType =
			type == GL_FLOAT		? AttributeType::Float
			: type == GL_FLOAT_VEC2 ? AttributeType::Vec2
			: type == GL_FLOAT_VEC3 ? AttributeType::Vec3
			: type == GL_FLOAT_VEC4 ? AttributeType::Vec4
									: AttributeType::None;

		if (attrType == AttributeType::Float) {
			if (std::strcmp("aDrawID", name) == 0) {
				_shader_drawIdOffset = totalSize;
			} else if (std::strcmp("aTexture", name) == 0) {
				_shader_textureIdOffset = totalSize;
			}
		} else if (attrType == AttributeType::Vec2) {
			if (std::strcmp("aPos", name) == 0) {
				_shader_positionOffset = totalSize;
				_shader_positionHasZ = false;
			} else if (std::strcmp("aUV", name) == 0) {
				_shader_uvOffset = totalSize;
			}
		} else if (attrType == AttributeType::Vec3) {
			if (std::strcmp("aPos", name) == 0) {
				_shader_positionOffset = totalSize;
				_shader_positionHasZ = true;
			} else if (std::strcmp("aColor", name) == 0) {
				_shader_colorOffset = totalSize;
				_shader_colorHasAlpha = false;
			}
		} else if (attrType == AttributeType::Vec4) {
			if (std::strcmp("aColor", name) == 0) {
				_shader_colorOffset = totalSize;
				_shader_colorHasAlpha = true;
			}
		}

		totalSize += AttributeType_Size(attrType);
		_vao.SetAttribute(i, attrType);
	}
	_shader_totalAttributeSize = totalSize;

	_buffer.Bind();
	_buffer.BufferData(nullptr, _shader_totalAttributeSize * MAX_VERTEX(_maxQuad), BufferUsage::DynamicDraw);
	_vertices.resize(_shader_totalAttributeSize * MAX_VERTEX(_maxQuad));

	_vao.UploadAttributes();
	_vao.Unbind();

	unsigned char pixel[] = {255, 255, 255, 255};
	_blankTexture = std::make_unique<ImageTexture>();
	_blankTexture->LoadFromData(pixel, 1, 1);

	std::cout << "_shader_positionOffset:     " << _shader_positionOffset << "\n";
	std::cout << "_shader_positionHasZ:       " << _shader_positionHasZ << "\n";
	std::cout << "_shader_uvOffset:           " << _shader_uvOffset << "\n";
	std::cout << "_shader_colorOffset:        " << _shader_colorOffset << "\n";
	std::cout << "_shader_colorHasAlpha:      " << _shader_colorHasAlpha << "\n";
	std::cout << "_shader_drawIdOffset:       " << _shader_drawIdOffset << "\n";
	std::cout << "_shader_textureIdOffset:    " << _shader_textureIdOffset << "\n";
	std::cout << "_shader_totalAttributeSize: " << _shader_totalAttributeSize << "\n";
}

void Renderer2D::PushVertex(DefaultVertex2D v) {
	if (_shader_positionOffset != -1) {
		_vertices[_verticesCursor + _shader_positionOffset + 0] = v.x;
		_vertices[_verticesCursor + _shader_positionOffset + 1] = v.y;

		if (_shader_positionHasZ)
			_vertices[_verticesCursor + _shader_positionOffset + 2] = v.z;
	}

	if (_shader_uvOffset != -1) {
		_vertices[_verticesCursor + _shader_uvOffset + 0] = v.u;
		_vertices[_verticesCursor + _shader_uvOffset + 1] = v.v;
	}

	if (_shader_colorOffset != -1) {
		_vertices[_verticesCursor + _shader_colorOffset + 0] = v.r;
		_vertices[_verticesCursor + _shader_colorOffset + 1] = v.g;
		_vertices[_verticesCursor + _shader_colorOffset + 2] = v.b;

		if (_shader_colorHasAlpha)
			_vertices[_verticesCursor + _shader_colorOffset + 3] = v.a;
	}

	if (_shader_drawIdOffset != -1) {
		_vertices[_verticesCursor + _shader_drawIdOffset] = v.d_id;
	}

	if (_shader_textureIdOffset != -1) {
		_vertices[_verticesCursor + _shader_textureIdOffset] = v.t_id;
	}

	_verticesCursor += _shader_totalAttributeSize;
}

 */