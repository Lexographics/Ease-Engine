#ifndef MESH_HPP
#define MESH_HPP
#pragma once

#include <cstdint>
#include <vector>

#include "core/resource.hpp"
#include "visual/model.hpp"

class Mesh : public Resource {
  public:
	Mesh();
	virtual ~Mesh();

	void Load(const char *path);

	void Draw() const;

  private:
	Model _model;
};

#endif // MESH_HPP