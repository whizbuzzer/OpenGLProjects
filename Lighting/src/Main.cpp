/* Main file for generating 3D graphics with lighting */

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

/* To create a pyramid
 * Coordinates are normalized, so they gotta be in [-1, 1]
 * Vertices are duplicated because normals will be different on each side of pyramid
 * Using face normals/flat shading which is more suited for flatter meshes such as cubes and pyramids:
 */
GLfloat vertices[] = {
	// COORDINATES     / COLORS                 / TEXTURE COORDS /  NORMALS				//
	// Bottom face
	-0.5f,  0.0f,  0.5f,     0.83f, 0.70f, 0.44f,     0.0f, 0.0f,     0.0f, -1.0f,  0.0f,  
	-0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     0.0f, 5.0f,     0.0f, -1.0f,  0.0f,
	 0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     5.0f, 5.0f,     0.0f, -1.0f,  0.0f,
	 0.5f,  0.0f,  0.5f,     0.83f, 0.70f, 0.44f,     5.0f, 0.0f,     0.0f, -1.0f,  0.0f,
						     						   				  
	 // Left face		     						   				  
	-0.5f,  0.0f,  0.5f,     0.83f, 0.70f, 0.44f,     0.0f, 0.0f,    -0.8f,  0.5f,  0.0f,
	-0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     5.0f, 0.0f,    -0.8f,  0.5f,  0.0f,
	 0.0f,  0.8f,  0.0f,     0.92f, 0.86f, 0.76f,     2.5f, 5.0f,    -0.8f,  0.5f,  0.0f,
	 
	 // Non-facing/Back face:
	-0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     5.0f, 0.0f,     0.0f,  0.5f, -0.8f,
	 0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     0.0f, 0.0f,     0.0f,  0.5f, -0.8f,
	 0.0f,  0.8f,  0.0f,     0.92f, 0.86f, 0.76f,     2.5f, 5.0f,     0.0f,  0.5f, -0.8f,

	 // Right face:
	 0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     0.0f, 0.0f,     0.8f,  0.5f,  0.0f,
	 0.5f,  0.0f,  0.5f,     0.83f, 0.70f, 0.44f,     5.0f, 0.0f,     0.8f,  0.5f,  0.0f,
	 0.0f,  0.8f,  0.0f,     0.92f, 0.86f, 0.76f,     2.5f, 5.0f,     0.8f,  0.5f,  0.0f,

	 // Facing/Front face:
	-0.5f,  0.0f,  0.5f,     0.83f, 0.70f, 0.44f,     5.0f, 0.0f,     0.0f,  0.5f,  0.8f,
	 0.5f,  0.0f,  0.5f,     0.83f, 0.70f, 0.44f,     0.0f, 0.0f,     0.0f,  0.5f,  0.8f,
	 0.0f,  0.8f,  0.0f,     0.92f, 0.86f, 0.76f,     2.5f, 5.0f,     0.0f,  0.5f,  0.8f
};  // It's okay if normals don't have unit length here as they will be normalized later in the shader

GLuint indices[] = {
	// Bottom face
	0, 1, 2,
	0, 2, 3,

	// Left face
	4, 6, 5,

	// Non-facing/Back face
	7, 9, 8,

	// Right face
	10, 12, 11,

	// Facing/Front face
	13, 15, 14
};

// Cube to serve as our light source:
GLfloat lightVertices[] = {
	// COORDINATES	   //
	-0.1f, -0.1f,  0.1f,  // 0
	-0.1f, -0.1f, -0.1f,  // 1
	 0.1f, -0.1f, -0.1f,  // 2
	 0.1f, -0.1f,  0.1f,  // 3
	-0.1f,  0.1f,  0.1f,  // 4
	-0.1f,  0.1f, -0.1f,  // 5
	 0.1f,  0.1f, -0.1f,  // 6
	 0.1f,  0.1f,  0.1f	  // 7
};

GLuint lightIndices[] = {
	// Bottom face
	0, 1, 2,
	0, 2, 3,

	// Front face
	0, 4, 7,
	0, 7, 3,

	// Right face
	3, 7, 6,
	3, 6, 2,

	// Back face
	2, 6, 5,
	2, 5, 1,

	// Left face
	1, 5, 4,
	1, 4, 0,

	// Top face
	4, 5, 6,
	4, 6, 7
};

int main() {
	glfwInit();

	// Setting up glfw:
	// To let glfw know which OpenGL version we are using since it does not know otherwise:
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Check Note.md

	// width x height pixels:
	GLFWwindow* window = glfwCreateWindow(width, height, "CameraAndLighting", NULL, NULL);

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

	/* Shader for 3D object */
	Shader shaderProgram("../shaders/lighting.vert", "../shaders/lighting.frag");

	/*Refer to Note.md for more info on Vertex Buffer Object (VBO),
	 * Vertex Array Object (VAO) and Element Array Buffer Object (EBO):
	 */
	 // ALWAYS GENERATE VAO BEFORE VBO!!!
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	// Linking vertex attributes:
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);					// Vertices
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));  // Colors
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));  // Textures
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));  // Normals
	
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	/* Shader for light cube */
	Shader lightShader("../shaders/light.vert", "../shaders/light.frag");

	VAO lightVAO;
	lightVAO.Bind();

	VBO lightVBO(lightVertices, sizeof(lightVertices));
	EBO lightEBO(lightIndices, sizeof(lightIndices));

	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);  // No colors for light (yet)
	
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);  // RBGA, normalized values.
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);  // Why???

	// Activating shaders and gaining transformation matrices of individual objects:
	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	// We don't want light effects just from the light cube, but also have it reflcct on the pyramid:
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	Texture pyramidTex("../data/sad_pepe.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	pyramidTex.texUnit(shaderProgram, "tex0", 0);
	// OpenGl reads images from bottom-left->top-right, but stb reads images from top-left->bottom-right

	// Read "Textures" in Notes.md

	// To tell OpenGL which triangle to put on top of which and avoid glitches:
	glEnable(GL_DEPTH_TEST);

	// Camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// For color alternating:
	float prev_time = float(glfwGetTime());
	float angle = 0.0f;

	// Making sure that the window does not close instantly:
	while (!glfwWindowShouldClose(window)) {

		float curr_time = float(glfwGetTime());
		if (curr_time - prev_time >= 0.1f) {
			angle += 0.1f;
			prev_time = curr_time;
		}

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);  // Background color Navy-blue (RGBA)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			  // Cleaning back buffer and assigning it new color

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		shaderProgram.Activate();
		glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), sin(angle), cos(angle), tan(angle), lightColor.w);
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(shaderProgram, "camMatrix");
		pyramidTex.Bind();
		VAO1.Bind();

		// Drawing our object:
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		lightShader.Activate();
		glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), sin(angle), cos(angle), tan(angle), lightColor.w);  // Changing color

		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);				  // To update graphics

		glfwPollEvents();  // Processes all pending events to prevent non-responsiveness
	}

	// Deleting objects to avoid memory leak:
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	pyramidTex.Delete();
	shaderProgram.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();  // Each initialize should be accompanied with a terminate

	return 0;
}