#ifndef BUFFER_HPP
#define BUFFER_HPP
#pragma once

#include <glad/glad.h>

enum class BufferType {
	None = GL_NONE,
	VertexBuffer = GL_ARRAY_BUFFER,
	IndexBuffer = GL_ELEMENT_ARRAY_BUFFER,
};

enum class BufferUsage {
	StaticDraw = GL_STATIC_DRAW,
	DynamicDraw = GL_DYNAMIC_DRAW,
};

class Buffer {
  public:
	Buffer() = default;
	Buffer(BufferType type);

	~Buffer();

	void New(BufferType type);
	void Delete();

	void Bind();
	void Unbind();

	void BufferData(const void *data, uint32_t size, BufferUsage usage);
	void BufferSubdata(const void *data, uint32_t size, uint32_t offset);

  private:
	uint32_t _id = 0;
	BufferType _bufferType = BufferType::None;
};

#endif // BUFFER_HPP