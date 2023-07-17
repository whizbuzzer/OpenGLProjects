/* Main file for generating 3D graphics */

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "EBO.h"
#include "shaderClass.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"

// Aspect ratio:
const unsigned int width = 800, height = 800;

/* Every 3 floats will represent one coordinate
 * Coordinates are normalized, so they gotta be in [-1, 1]:
 */

// To creat a pyramid:
GLfloat vertices[] = {
	// COORDINATES          / COLORS                / COORDINATES TO MAP TEXTURE ONTO VERTICES  //
		-0.5f,  0.0f,  0.5f,   0.83f,  0.70f, 0.44f,    0.0f, 0.0f,  // anterior-left
		-0.5f,  0.0f, -0.5f,   0.83f,  0.70f, 0.44f,    5.0f, 0.0f,  // posterior-left
		 0.5f,  0.0f, -0.5f,   0.83f,  0.70f, 0.44f,    0.0f, 0.0f,  // posterior-right
		 0.5f,  0.0f,  0.5f,   0.83f,  0.70f, 0.44f,    5.0f, 0.0f,  // anterior-right
		 0.0f,  0.8f,  0.0f,   0.92f,  0.86f, 0.76f,    2.5f, 5.0f   // top
};

GLuint indices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

int main() {
	glfwInit();

	// Setting up glfw:
	// To let glfw know which OpenGL version we are using since it does not know otherwise:
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Check Note.md

	// width x height pixels:
	GLFWwindow* window = glfwCreateWindow(width, height, "CameraAnd3D", NULL, NULL);
	
	// In case window fails to create:
	if (window == NULL) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Adding window to context(making it part of OpenGL current state):
	glfwMakeContextCurrent(window);

	gladLoadGL();  // Loads needed configurations for OpenGL
	
	// Bottom-left to top-right https://registry.khronos.org/OpenGL-Refpages/gl4/html/glViewport.xhtml
	glViewport(0, 0, width, height);

	Shader shaderProgram("../shaders/camera3d.vert", "../shaders/camera3d.frag");

	/*Refer to Note.md for more info on Vertex Buffer Object (VBO),
	 * Vertex Array Object (VAO) and Element Array Buffer Object (EBO):
	 */
	 // ALWAYS GENERATE VAO BEFORE VBO!!!
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);					   // Vertices
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));  // Colors
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));  // Texture
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	Texture pyramidTex("../data/sad_pepe.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	pyramidTex.texUnit(shaderProgram, "tex0", 0);
	// OpenGl reads images from bottom-left->top-right, but stb reads images from top-left->bottom-right

	// Read "Textures" in Notes.md

	// To tell OpenGL which triangle to put on top of which and avoid glitches:
	glEnable(GL_DEPTH_TEST);

	// Camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Making sure that the window does not close instantly:
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);  // Background color Navy-blue (RGBA)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			  // Cleaning back buffer and assigning it new color
		
		shaderProgram.Activate();

		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
		
		pyramidTex.Bind();

		VAO1.Bind();

		// Drawing our object:
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);				  // To update graphics

		glfwPollEvents();  // Processes all pending events to prevent non-responsiveness
	}

	// Deleting objects to avoid memory leak:
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	pyramidTex.Delete();
	shaderProgram.Delete();
	
	glfwDestroyWindow(window);
	glfwTerminate();  // Each initialize should be accompanied with a terminate

	return 0;
}