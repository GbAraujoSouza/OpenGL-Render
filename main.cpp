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
#include "cube.h"

#define VIEW_WIDTH 800
#define VIEW_HEIGHT 600

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

	GLFWwindow* window = glfwCreateWindow(VIEW_WIDTH, VIEW_HEIGHT, "OpenGL de cria", NULL, NULL);
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
	glEnable(GL_DEPTH_TEST);

	// resize window viewport
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	Shader programShader("shaders/vert.glsl", "shaders/frag.glsl");
	programShader.use();

	// SETUP VERTEX ATTRIBUTES ######################

	VAO vao{};
	vao.bind();

	VBO vbo(cube::vertices, sizeof(GLfloat) * cube::vertices.size());
	//EBO ebo(indices, sizeof(indices));


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
	//ebo.unbind(); // need to unbind EBO after the VAO


	// SETUP TEXTURES ###################
	Texture texture1("container.jpg", GL_RGB);
	Texture texture2("awesomeface.png", GL_RGBA);

	programShader.use();
	programShader.setInt("texture0", 0);
	programShader.setInt("texture1", 1);

	std::vector<glm::vec3> cubePositions = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture1.activate(GL_TEXTURE0);
		texture1.bind();

		texture2.activate(GL_TEXTURE1);
		texture2.bind();

		programShader.use();

		// rotate with time
		float t = glfwGetTime();

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		programShader.setMat4("view", view);

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), static_cast<float>(VIEW_WIDTH) / VIEW_HEIGHT, 0.1f, 100.0f);
		programShader.setMat4("projection", projection);


		vao.bind();
		//ebo.bind();

		for (auto pos : cubePositions) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pos);
			model = glm::rotate(model, glm::radians(-30.0f) * t, glm::vec3(1.0f, 1.0f, 0.0f));
			programShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		vao.unbind();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	programShader.deleteShader();

	vao.delete_buffer();
	vbo.delete_buffer();
	//ebo.delete_buffer();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}