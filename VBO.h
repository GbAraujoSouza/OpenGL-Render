#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include <vector>

class VBO {
public:
	GLuint id;
	VBO(std::vector<GLfloat>& vertices, GLsizeiptr size);

	void bind();
	void unbind();
	void delete_buffer();
};

#endif // !VBO_CLASS_H

