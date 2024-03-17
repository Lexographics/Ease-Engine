#include "vertex_array.hpp"

#include <algorithm>

VertexArray::~VertexArray() {
	Delete();
}

void VertexArray::New() {
	Delete();

	glGenVertexArrays(1, &_id);
}

void VertexArray::Delete() {
	if (_id != 0) {
		glDeleteVertexArrays(1, &_id);
		_id = 0;
	}
}

void VertexArray::Bind() {
	glBindVertexArray(_id);
}

void VertexArray::Unbind() {
	glBindVertexArray(0);
}

void VertexArray::ResetAttributes() {
	for (const VertexArrayAttribute &attribute : _attributes) {
		glDisableVertexAttribArray(attribute.slot);
	}

	_attributeSizeInBytes = 0;
	_attributes.clear();
}

void VertexArray::SetAttribute(uint32_t slot, AttributeType type) {
	_attributes.emplace_back(slot, type);
	_attributeSizeInBytes += AttributeType_Size(type);
}

void VertexArray::UploadAttributes() {
	std::sort(_attributes.begin(), _attributes.end());

	uint64_t bytes = 0;
	for (const VertexArrayAttribute &attribute : _attributes) {
		glVertexAttribPointer(
			attribute.slot,
			AttributeType_ComponentCount(attribute.type),
			AttributeType_GLenum(attribute.type),
			false,
			_attributeSizeInBytes,
			(const void *)bytes);

		glEnableVertexAttribArray(attribute.slot);
		bytes += AttributeType_Size(attribute.type);
	}
}

void VertexArray::DisableAtributes() {
	for (const VertexArrayAttribute &attribute : _attributes) {
		glDisableVertexAttribArray(attribute.slot);
	}
}