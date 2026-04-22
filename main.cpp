#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "stb_image.h"

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		glClearColor(0.949f, 0.835f, 0.812f, 1.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		glClearColor(0.6f, 0.82f, 0.858f, 1.0f);
	}
}

int main() {

	/////////// SETUP WINDOW //////////////////
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL de cria", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	// resize window viewport
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	Shader programShader("shaders/shader.vs", "shaders/shader.fs");
	programShader.use();
	programShader.setFloat("positionOffset", 0.0f);


	// SETUP VERTEX ATTRIBUTES ######################
	float vertices[] = {
		//first
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
		-0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // top left 
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom left
	};
	unsigned int indices[] = {
		0, 1, 3,
		0, 2, 3
	};

	VAO vao{};
	vao.bind();

	VBO vbo(vertices, sizeof(vertices));
	EBO ebo(indices, sizeof(indices));


	// Configure VERTEX ATTRIBUTES ###########
	// position attribute
	vao.linkVBO(vbo, 0, 3,(void*)0);
	// color attribute
	vao.linkVBO(vbo, 1, 3,(void*)(3 * sizeof(GLfloat)));
	// texture attribute
	vao.linkVBO(vbo, 2, 2,(void*)(6* sizeof(GLfloat)));


	// Unbind buffers
	vbo.unbind();
	vao.unbind();
	ebo.unbind(); // need to unbind EBO after the VAO


	// SETUP TEXTURES ###################
	stbi_set_flip_vertically_on_load(true);
	unsigned int texturesIDs[2];
	int width, height, nrChannels;
	unsigned char* data;

	glGenTextures(2, texturesIDs);

	glBindTexture(GL_TEXTURE_2D, texturesIDs[0]);
	data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR::TEXTURE::FAILED_TO_READ_TEXTURE" << "\n";
	}

	glBindTexture(GL_TEXTURE_2D, texturesIDs[1]);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "ERROR::TEXTURE::FAILED_TO_READ_TEXTURE" << "\n";
	}

	stbi_image_free(data);

	programShader.use();
	programShader.setInt("texture0", 0);
	programShader.setInt("texture1", 1);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturesIDs[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texturesIDs[1]);
		programShader.use();

		vao.bind();
		ebo.bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		vao.unbind();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	programShader.deleteShader();

	vao.delete_buffer();
	vbo.delete_buffer();
	ebo.delete_buffer();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}