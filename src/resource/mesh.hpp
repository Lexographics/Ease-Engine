#ifndef MESH_HPP
#define MESH_HPP
#pragma once

#include <cstdint>
#include <vector>

#include "visual/model.hpp"

class Mesh {
  public:
	Mesh() = default;
	~Mesh();

	void Load(const char *path);

	void Draw() const;

  private:
	Model _model;
};

#endif // MESH_HPP