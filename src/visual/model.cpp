#include "model.hpp"

Model::~Model() {
	Delete();
}

void Model::SetModelData(const std::vector<float> &data) {
	_vboSize = data.size();

	_vao.Bind();
	_vbo.Bind();
	_vbo.BufferData(data.data(), data.size() * sizeof(float), BufferUsage::StaticDraw);
	_vao.Unbind();
}

void Model::SetIndexData(const std::vector<uint32_t> &data) {
	_indexCount = data.size();

	_vao.Bind();
	_ebo.Bind();
	_ebo.BufferData(data.data(), data.size() * sizeof(uint32_t), BufferUsage::StaticDraw);
	_vao.Unbind();
}

void Model::ResetAttributes() {
	_vao.Bind();
	_vao.ResetAttributes();
	_vao.Unbind();
}

void Model::SetAttribute(uint32_t slot, AttributeType type) {
	_vao.SetAttribute(slot, type);
}

void Model::UploadAttributes() {
	_vao.Bind();
	_vbo.Bind();
	if (_indexCount > 0)
		_ebo.Bind();
	_vao.UploadAttributes();
	_vao.Unbind();
}

void Model::New() {
	Delete();

	_vao.New();
	_vbo.New(BufferType::VertexBuffer);
	_ebo.New(BufferType::IndexBuffer);
}

void Model::Delete() {
	_vao.Delete();
	_vbo.Delete();
	_ebo.Delete();
}

void Model::Draw() const {
	_vao.Bind();
	if (_indexCount > 0)
		glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, nullptr);
	else if (_vao.SizeInBytes() > 0)
		glDrawArrays(GL_TRIANGLES, 0, _vboSize);
	_vao.Unbind();
}