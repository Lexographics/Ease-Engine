#include "mesh.hpp"

#include <iostream>
#include <vector>

#include "visual/gl.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "core/application.hpp"

Mesh::~Mesh() {
}

void Mesh::Load(const char *path) {
	_model.New();

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	std::string warn;

	auto file = App().FS()->Load(path);
	if (!file) {
		return;
	}
	std::string data{reinterpret_cast<char *>(file->buffer.data()), file->buffer.size()};
	std::istringstream ss(data);

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &ss);

	if (!err.empty()) {
		std::cout << "Failed to load object file " << path << std::endl;
		std::cout << "error: " << err << std::endl;
		return;
	}

	std::vector<float> vertexData;
	std::vector<unsigned int> indexData;
	bool hasNormals = false;
	bool hasTexCoords = false;

	(void)hasNormals;
	(void)hasTexCoords;

	for (size_t s = 0; s < shapes.size(); s++) {
		size_t indexOffset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			for (size_t v = 0; v < fv; v++) {
				tinyobj::index_t idx = shapes[s].mesh.indices[indexOffset + v];
				indexData.push_back(indexData.size());

				tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				vertexData.push_back(vx);
				vertexData.push_back(vy);
				vertexData.push_back(vz);

				if (idx.normal_index >= 0) {
					tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
					tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
					tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];

					vertexData.push_back(nx);
					vertexData.push_back(ny);
					vertexData.push_back(nz);

					hasNormals = true;
				} else {
					vertexData.push_back(0.f);
					vertexData.push_back(0.f);
					vertexData.push_back(0.f);
				}

				if (idx.texcoord_index >= 0) {
					tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					tinyobj::real_t ty = 1.f - attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];

					vertexData.push_back(tx);
					vertexData.push_back(ty);

					hasTexCoords = true;
				} else {
					vertexData.push_back(0.f);
					vertexData.push_back(0.f);
				}

				// Optional: vertex colors
				// tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
				// tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
				// tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
			}
			indexOffset += fv;

			// per-face material
			// shapes[s].mesh.material_ids[f];
		}
	}

	_model.ResetAttributes();
	_model.SetModelData(vertexData);
	_model.SetIndexData(indexData);
	_model.SetAttribute(0, AttributeType::Vec3);
	_model.SetAttribute(1, AttributeType::Vec3);
	_model.SetAttribute(2, AttributeType::Vec2);
	_model.UploadAttributes();
}

void Mesh::Draw() const {
	_model.Draw();
}