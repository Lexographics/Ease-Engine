#ifndef GL_HPP
#define GL_HPP
#pragma once

#define GLFW_INCLUDE_NONE

#ifdef SW_WEB
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <emscripten.h>
#else
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#endif

#endif // GL_HPP