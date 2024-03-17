#include "shader.hpp"

#include "visual/visual.hpp"
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>

static std::string ReadFile(const char *path) {
	std::ifstream ifstream(path);
	std::stringstream ss;
	ss << ifstream.rdbuf();
	return ss.str();
}

static bool HandleCompileError(uint32_t id, const char *shaderType) {
	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << shaderType << " shader error:"
				  << infoLog << std::endl;

		return false;
	}
	return true;
}

static bool HandleLinkError(uint32_t id) {
	int success;
	char infoLog[512];
	glGetProgramiv(id, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "Shader program compilation failed"
				  << infoLog << std::endl;
		return false;
	}
	return true;
}

Shader::~Shader() {
	Delete();
}

void Shader::Load(const char *path) {
	Delete();

	std::string vertexSource = ReadFile((std::string(path) + ".vs").c_str());
	std::string fragmentSource = ReadFile((std::string(path) + ".fs").c_str());

	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char *vertexSrc = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
	glCompileShader(vertexShader);
	if (!HandleCompileError(vertexShader, "vertex")) {
		return;
	}

	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char *fragmentSrc = fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
	glCompileShader(fragmentShader);
	if (!HandleCompileError(fragmentShader, "fragment")) {
		return;
	}

	_id = glCreateProgram();
	glAttachShader(_id, vertexShader);
	glAttachShader(_id, fragmentShader);

	glLinkProgram(_id);
	if (!HandleLinkError(_id)) {
		return;
	}

	return;
}

void Shader::Delete() {
	if (_id != 0 && Visual::Active()) {
		glDeleteProgram(_id);
		_id = 0;
	}
}

void Shader::Bind() {
	glUseProgram(_id);
}

void Shader::Unbind() {
	glUseProgram(0);
}

void Shader::Uniformiv(const char *name, const std::vector<int> &value) {
	Bind();
	int loc = glGetUniformLocation(_id, name);
	if (loc < 0) {
		return;
	}

	glUniform1iv(loc, value.size(), value.data());
}

void Shader::UniformMat4(const char *name, const glm::mat4 &value) {
	Bind();
	int loc = glGetUniformLocation(_id, name);
	if (loc < 0) {
		return;
	}

	glUniformMatrix4fv(loc, 1, GL_FALSE, &value[0][0]);
}

void Shader::UniformTexture(const char *name, uint32_t id, int slot /*= 0*/) {
	Bind();
	int loc = glGetUniformLocation(_id, name);
	if (loc < 0) {
		return;
	}

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
	glUniform1i(loc, slot);
}
