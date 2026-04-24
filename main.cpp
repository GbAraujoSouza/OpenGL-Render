#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"

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

	Shader programShader("shaders/vert.glsl", "shaders/frag.glsl");
	programShader.use();

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
	vao.linkVBO(vbo, 0, 3, (void*)0);
	// color attribute
	vao.linkVBO(vbo, 1, 3, (void*)(3 * sizeof(GLfloat)));
	// texture attribute
	vao.linkVBO(vbo, 2, 2, (void*)(6 * sizeof(GLfloat)));


	// Unbind buffers
	vbo.unbind();
	vao.unbind();
	ebo.unbind(); // need to unbind EBO after the VAO


	// SETUP TEXTURES ###################
	Texture texture1("container.jpg", GL_RGB);
	Texture texture2("awesomeface.png", GL_RGBA);

	programShader.use();
	programShader.setInt("texture0", 0);
	programShader.setInt("texture1", 1);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT);

		texture1.activate(GL_TEXTURE0);
		texture1.bind();

		texture2.activate(GL_TEXTURE1);
		texture2.bind();

		programShader.use();

		glm::mat4 transformMatrix = glm::mat4(1.0f);
		transformMatrix = glm::translate(transformMatrix, glm::vec3(0.5, 0.5, 0.0));
		transformMatrix = glm::rotate(transformMatrix, 2.0f * static_cast<float>(glfwGetTime()), glm::vec3(0.0, 0.0, 1.0));

		GLuint transformMatrixLoc = glGetUniformLocation(programShader.ID, "transformMatrix");
		glUniformMatrix4fv(transformMatrixLoc, 1, GL_FALSE, glm::value_ptr(transformMatrix));


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