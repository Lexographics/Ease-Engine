#ifndef MODEL_HPP
#define MODEL_HPP
#pragma once

#include "gl/buffer.hpp"
#include "gl/vertex_array.hpp"

#include <cstdint>
#include <vector>

class Model {
  public:
	Model() = default;
	~Model();

	void SetModelData(const std::vector<float> &data);
	void SetIndexData(const std::vector<uint32_t> &data);

	void ResetAttributes();
	void SetAttribute(uint32_t slot, AttributeType type);
	void UploadAttributes();

	void New();
	void Delete();

	void Draw() const;

  private:
	VertexArray _vao;
	Buffer _vbo;
	Buffer _ebo;

	uint32_t _indexCount = 0;
	uint32_t _vboSize = 0;
};

#endif // MODEL_HPP