#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	// shader program id
	GLuint ID;

	Shader(const char* vertexPath, const char* fragmentPath);
	
	void use() const;

	void deleteShader() const;

	// util functions to set uniforms
	void setbool(const std::string& uniformName, bool value) const;

	void setInt(const std::string& uniformName, int value) const;

	void setFloat(const std::string& uniformName, float value) const;

	void setMat4(const std::string& uniformName, glm::mat4& matrix) const;
};

#endif // !SHADER_H

