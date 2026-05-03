#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << "\n";
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << "\n";
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << "\n";
	}

	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertex);
	glAttachShader(this->ID, fragment);
	glLinkProgram(this->ID);
	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << infoLog << "\n";
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() const {
	glUseProgram(this->ID);
}

void Shader::deleteShader() const {
	glUseProgram(this->ID);
}

// util functions to set uniforms
void Shader::setbool(const std::string& uniformName, bool value) const {
	// uniform do not accept bool type, so we convert to a int value {0: false, 1: true}
	GLuint uniformID = glGetUniformLocation(this->ID, uniformName.c_str());
	glUniform1i(uniformID, (int)value);
}

void Shader::setInt(const std::string& uniformName, int value) const {
	GLuint uniformID = glGetUniformLocation(this->ID, uniformName.c_str());
	glUniform1i(uniformID, value);
}

void Shader::setFloat(const std::string& uniformName, float value) const {
	GLuint uniformID = glGetUniformLocation(this->ID, uniformName.c_str());
	glUniform1f(uniformID, value);
}

void Shader::setMat4(const std::string& uniformName, glm::mat4& matrix) const {
	GLuint uniformID = glGetUniformLocation(this->ID, uniformName.c_str());
	glUniformMatrix4fv(uniformID, 1, GL_FALSE, glm::value_ptr(matrix));
}
