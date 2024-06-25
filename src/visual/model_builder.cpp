#include "model_builder.hpp"

#define POS(x, y, z) x, y, z
#define UV(u, v) u, v
#define COLOR(r, g, b, a) r, g, b, a
#define ID(id) id

bool ModelBuilder::Quad2D(Model &model, float size /*= 1.f*/) {
	model.New();

	float sz = size / 2.f;

	const std::vector<float> vertices = {
		POS(sz, sz, 0.0f), /****/ UV(1.0, 1.0), // top right
		POS(sz, -sz, 0.0f), /***/ UV(1.0, 0.0), // bottom right
		POS(-sz, -sz, 0.0f), /**/ UV(0.0, 0.0), // bottom left
		POS(-sz, sz, 0.0f), /***/ UV(0.0, 1.0)	// top left
	};

	const std::vector<uint32_t> indices = {
		0, 1, 3,
		1, 2, 3};

	model.SetModelData(vertices);
	model.SetIndexData(indices);

	model.ResetAttributes();
	model.SetAttribute(0, AttributeType::Vec3);
	model.SetAttribute(1, AttributeType::Vec2);
	model.UploadAttributes();

	return true;
}

bool ModelBuilder::Quad2D(Model &model, float x, float y, float w, float h) {
	model.New();

	const std::vector<float> vertices = {
		POS(x + w, y + h, 0.0f), /**/ UV(1.0, 1.0), // top right
		POS(x + w, y, 0.0f), /******/ UV(1.0, 0.0), // bottom right
		POS(x, y, 0.0f), /**********/ UV(0.0, 0.0), // bottom left
		POS(x, y + h, 0.0f), /******/ UV(0.0, 1.0)	// top left
	};

	const std::vector<uint32_t> indices = {
		0, 1, 3,
		1, 2, 3};

	model.SetModelData(vertices);
	model.SetIndexData(indices);

	model.ResetAttributes();
	model.SetAttribute(0, AttributeType::Vec3);
	model.SetAttribute(1, AttributeType::Vec2);
	model.UploadAttributes();

	return true;
}