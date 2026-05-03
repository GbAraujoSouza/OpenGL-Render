#include "VAO.h"

VAO::VAO() {
	glGenVertexArrays(1, &this->id);
}

void VAO::linkVBO(VBO vbo, GLuint layout, GLint attributeSize, const void* pointer) {
	vbo.bind();
	// parameters:
	// 1. index(layout) to generic vertex attrubute to modify (ex: layout 0 -> pos; layout 1 -> color)
	// 2. size of the component (pos -> 3; color -> 3; texture -> 2)
	// 3. type of data
	// 4. should normilize
	// 5. stride -> offset between two vertex attributes (ex: given a x pos, how may elements i find another?)
	// 6. pointer -> offset of the first of the first vertex attribute component
		// offset of the first possition, color or textPos
	glVertexAttribPointer(layout, attributeSize, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), pointer);
	glEnableVertexAttribArray(layout);
	vbo.unbind();
}

void VAO::bind() {
	glBindVertexArray(this->id);
}
void VAO::unbind() {
	glBindVertexArray(0);
}
void VAO::delete_buffer() {
	glDeleteVertexArrays(1, &this->id);
}
