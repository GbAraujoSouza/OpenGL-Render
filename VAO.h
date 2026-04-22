#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:
	GLuint id;
	VAO();

	void linkVBO(VBO vbo, GLuint layout, GLint attributeSize,const void* pointer);

	void bind();
	void unbind();
	void delete_buffer();
};

#endif // !VAO_CLASS_H

