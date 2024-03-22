#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP
#pragma once

#include <cstdint>
#include <glad/glad.h>
#include <vector>

enum class AttributeType {
	Float = 0,
	Vec2,
	Vec3,
	Vec4,
};

inline GLenum AttributeType_GLenum(AttributeType type) {
	return type == AttributeType::Float	 ? GL_FLOAT
		   : type == AttributeType::Vec2 ? GL_FLOAT
		   : type == AttributeType::Vec3 ? GL_FLOAT
		   : type == AttributeType::Vec4 ? GL_FLOAT
										 : GL_NONE;
}

inline int AttributeType_ComponentCount(AttributeType type) {
	return type == AttributeType::Float	 ? 1
		   : type == AttributeType::Vec2 ? 2
		   : type == AttributeType::Vec3 ? 3
		   : type == AttributeType::Vec4 ? 4
										 : 0;
}

inline int AttributeType_Size(AttributeType type) {
	return type == AttributeType::Float	 ? sizeof(GLfloat)
		   : type == AttributeType::Vec2 ? sizeof(GLfloat) * 2
		   : type == AttributeType::Vec3 ? sizeof(GLfloat) * 3
		   : type == AttributeType::Vec4 ? sizeof(GLfloat) * 4
										 : 0;
}

struct VertexArrayAttribute {
	AttributeType type;
	uint32_t slot;

	VertexArrayAttribute(uint32_t slot, AttributeType type) : type(type), slot(slot) {}

	bool operator<(const VertexArrayAttribute &other) const {
		return slot < other.slot;
	}
};

class VertexArray {
  public:
	VertexArray() = default;
	~VertexArray();

	void New();
	void Delete();

	void Bind() const;
	void Unbind() const;

	void ResetAttributes();
	void SetAttribute(uint32_t slot, AttributeType type);
	void UploadAttributes();
	void DisableAtributes();

	inline uint32_t SizeInBytes() const { return _attributeSizeInBytes; }

  private:
	uint32_t _id = 0;

	uint32_t _attributeSizeInBytes = 0;
	std::vector<VertexArrayAttribute> _attributes;
};

#endif // VERTEX_ARRAY_HPP