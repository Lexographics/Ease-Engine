#ifndef RENDERER_HPP
#define RENDERER_HPP
#pragma once

#include <glm/glm.hpp>

#include "model.hpp"
#include "resource/font.hpp"
#include "resource/mesh.hpp"

namespace Visual::Renderer {

void InitState();

void Reset();
void PushQuad(float x, float y, float w, float h, float r, float g, float b, float a, float drawID, float textureID);
void PushQuad(glm::mat4 transform, float textureID, float r = 1.f, float g = 1.f, float b = 1.f, float a = 1.f, float drawID = 1.f);
void End();

void DrawText(const std::string &text, Font &font, const glm::mat4 &transform);

void DrawFullscreen(uint32_t textureID);

void DrawMesh(const Mesh &mesh, const glm::mat4 &transform);

} // namespace Visual::Renderer

#endif // RENDERER_HPP