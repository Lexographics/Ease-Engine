#ifndef MODEL_BUILDER_HPP
#define MODEL_BUILDER_HPP
#pragma once

#include "model.hpp"

namespace ModelBuilder {
bool Quad2D(Model &model, float size = 1.f);
bool Quad2D(Model &model, float x, float y, float w, float h);
} // namespace ModelBuilder

#endif // MODEL_BUILDER_HPP