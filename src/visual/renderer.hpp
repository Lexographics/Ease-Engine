#ifndef RENDERER_HPP
#define RENDERER_HPP
#pragma once

#include <glm/glm.hpp>

namespace Visual::Renderer {

void InitState();

void Reset();
void PushQuad(float x, float y, float w, float h, float r, float g, float b, float a, float drawID, float textureID);
void PushQuad(glm::mat4 transform, float textureID, float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f, float drawID = 1.f);
void End();

void DrawFullscreen(uint32_t textureID);

} // namespace Visual::Renderer

#endif // RENDERER_HPP