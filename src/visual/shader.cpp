#include "shader.hpp"

#include "visual/gl.hpp"
#include "visual/visual.hpp"
#include <fstream>
#include <glm/glm.hpp>
#include <sstream>

#include "core/application.hpp"
#include "core/debug.hpp"

static bool HandleCompileError(uint32_t id, const char *shaderType) {
	int success;
	char infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		Debug::Error("{} shader error: {}", shaderType, infoLog);

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
		Debug::Error("Shader program compilation failed: {}", infoLog);
		return false;
	}
	return true;
}

Shader::~Shader() {
	Delete();
}

void Shader::Load(const char *vertexPath, const char *fragmentPath) {
	Delete();

	Ref<FileData> vertexFile = App().FS().Load(vertexPath);
	if (!vertexFile) {
		Debug::Error("Failed to load vertex shader at {}", vertexPath);
		return;
	}

	Ref<FileData> fragmentFile = App().FS().Load(fragmentPath);
	if (!fragmentFile) {
		Debug::Error("Failed to load fragment shader at {}", fragmentPath);
		return;
	}

	std::string vertexSource{reinterpret_cast<char *>(vertexFile->Data()), vertexFile->Size()};
	std::string fragmentSource{reinterpret_cast<char *>(fragmentFile->Data()), fragmentFile->Size()};

	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char *vertexSrc = vertexSource.c_str();
	glShaderSource(vertexShader, 1, &vertexSrc, nullptr);
	glCompileShader(vertexShader);
	if (!HandleCompileError(vertexShader, "vertex")) {
		Debug::Error("Failed to load shader: {}", vertexPath);
		return;
	}

	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char *fragmentSrc = fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
	glCompileShader(fragmentShader);
	if (!HandleCompileError(fragmentShader, "fragment")) {
		Debug::Error("Failed to load shader: {}", fragmentPath);
		return;
	}

	_id = glCreateProgram();
	glAttachShader(_id, vertexShader);
	glAttachShader(_id, fragmentShader);

	glLinkProgram(_id);
	if (!HandleLinkError(_id)) {
		Debug::Error("Failed to link shader: {}, {}", vertexPath, fragmentPath);
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
