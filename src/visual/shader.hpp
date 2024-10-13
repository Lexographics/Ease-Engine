#ifndef SHADER_HPP
#define SHADER_HPP
#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Shader {
  public:
	Shader() = default;
	~Shader();

	void Load(const char *vertexPath, const char *fragmentPath);

	void Delete();

	void Bind();
	void Unbind();

	void Uniformiv(const char *name, const std::vector<int> &value);
	void UniformFloat(const char *name, float value);
	void UniformVec2(const char *name, const glm::vec2 &value);
	void UniformMat4(const char *name, const glm::mat4 &value);
	void UniformTexture(const char *name, uint32_t id, int slot = 0);

	inline uint32_t ID() { return _id; }

  private:
	uint32_t _id = 0;
	std::string _vertexSource = "";
	std::string _fragmentSource = "";
};

#endif // SHADER_HPP