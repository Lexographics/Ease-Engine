#include "buffer.hpp"

Buffer::Buffer(BufferType type) {
	New(type);
}

Buffer::~Buffer() {
	Delete();
}

void Buffer::New(BufferType type) {
	Delete();

	_bufferType = type;
	glGenBuffers(1, &_id);
}

void Buffer::Delete() {
	if (_id != 0) {
		glDeleteBuffers(1, &_id);
		_id = 0;
	}
}

void Buffer::Bind() const {
	glBindBuffer((GLenum)_bufferType, _id);
}

void Buffer::Unbind() const {
	glBindBuffer((GLenum)_bufferType, 0);
}

void Buffer::BufferData(const void *data, uint32_t size, BufferUsage usage) {
	glBufferData((GLenum)_bufferType, size, data, (GLenum)usage);
}

void Buffer::BufferSubdata(const void *data, uint32_t size, uint32_t offset) {
	glBufferSubData((GLenum)_bufferType, offset, size, data);
}