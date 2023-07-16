/* Main file for generating 3D graphics */

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	GLFWwindow* window = glfwCreateWindow(width, height, "Render3D", NULL, NULL);
	
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

	Shader shaderProgram("../shaders/three_d.vert", "../shaders/three_d.frag");

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
	
	// ID of uniform-type variable "scale": 
	GLuint scaleID = glGetUniformLocation(shaderProgram.ID, "scale");

	Texture pyramidTex("../data/sad_pepe.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	pyramidTex.texUnit(shaderProgram, "tex0", 0);
	// OpenGl reads images from bottom-left->top-right, but stb reads images from top-left->bottom-right

	// Read "Textures" in Notes.md

	// Variables for rotating the pyramid:
	float rotation = 0.0f;
	double prev_time = glfwGetTime();

	// To tell OpenGL which triangle to put on top of which and avoid glitches:
	glEnable(GL_DEPTH_TEST);

	// Making sure that the window does not close instantly:
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);  // Background color Navy-blue (RGBA)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			  // Cleaning back buffer and assigning it new color
		
		shaderProgram.Activate();

		// Changing rotation angle every 60th of a second:
		double curr_time = glfwGetTime();
		if (curr_time - prev_time >= 1 / 60) {
			rotation += 0.01f;
			prev_time = curr_time;
		}

		// Initializing transformation matrices. Initializing matrices without inputs will create useless zero matrices:
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		/*glm::mat4 model, view, proj;
		for (auto tx : { model, view, proj }) {
			tx = glm::mat4(1.0f);
		}*/

		// Rotating object about Y-axis:
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));

		// We move the whole world around us instead of us moving around the world (Take that Copernicus!)
		view = glm::translate(view, glm::vec3(0.0f, -0.3f, -2.0));						      // Z-axis is positive towards us and negative away from us.
		proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);  // FOV, aspect ratio, closest point we can see, furthest point we can see
				
		// Initializing transform locations:
		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
		
		// Uniforms can be assigned value only after activating the shader:
		glUniform1f(scaleID, 0.0f);
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