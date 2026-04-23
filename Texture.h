#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <iostream>
#include "stb_image.h"

class Texture {
public:
	GLuint id;
	const char* textureFile;
	GLenum format;
	Texture(const char* textureFilePath, GLenum format);

	void bind();
	void activate(GLenum texture);
};

#endif // !TEXTURE_CLASS_H
