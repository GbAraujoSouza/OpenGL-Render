#include "Texture.h"

Texture::Texture(const char* textureFilePath, GLenum format) : textureFile(textureFilePath), format(format) {
	stbi_set_flip_vertically_on_load(true);
	glGenTextures(1, &this->id);
}

void Texture::bind() {
	int width, height, nrChannels;
	unsigned char* data;

	glBindTexture(GL_TEXTURE_2D, this->id);
	data = stbi_load(this->textureFile, &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR::TEXTURE::FAILED_TO_READ_TEXTURE" << "\n";
	}
	stbi_image_free(data);
}

void Texture::activate(GLenum texture) {
	glActiveTexture(texture);
}