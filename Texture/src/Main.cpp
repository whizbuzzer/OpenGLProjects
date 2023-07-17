/* Main file for generating sad_pepe texture */

//#include <filesystem>
//namespace fs = std::filesystem;  // Requires C++17 and above

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include "EBO.h"
#include "shaderClass.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"


/* All OpenGL objects are accessed by reference 
 */

/* Arbitrary coordinates for generating a 2D rectangle:
 * Every 3 floats will represent one coordinate
 * Centre of the coordinates for X and Y is the middle of the screen
 * Z coordinate would not be considered for 2D
 * Coordinates are normalized, so they gotta be in [-1, 1]:
 */

GLfloat n_textures = 2;  // Total number of textures will be the square of this
//GLfloat vertices[] = {
//	// COORDINATES        / COLORS             / COORDINATES TO MAP TEXTURE ONTO VERTICES  //
//		-0.5f, -0.5f, 0.0f,   1.0f,  0.0f, 0.0f,                 0.0f,              0.0f,  // Lower-left
//		-0.5f,  0.5f, 0.0f,   0.0f,  1.0f, 0.0f,                 0.0f, n_textures * 1.0f,  // Upper-left
//		 0.5f,  0.5f, 0.0f,   0.0f,  0.0f, 1.0f,    n_textures * 1.0f, n_textures * 1.0f,  // Upper-right
//		 0.5f, -0.5f, 0.0f,   1.0f,  1.0f, 0.0f,    n_textures * 1.0f,              0.0f   // Lower-right
//};

// To creat a triangle:
GLfloat vertices[] = {
	// COORDINATES        / COLORS             / COORDINATES TO MAP TEXTURE ONTO VERTICES  //
		-0.5f, -0.5f, 0.0f,   1.0f,  0.0f, 0.0f,                -1.0f,             -1.0f,  // Lower-left
		 0.0f,  0.5f, 0.0f,   0.0f,  1.0f, 0.0f,                 0.5f, n_textures * 1.0f,  // Upper
		 0.5f, -0.5f, 0.0f,   1.0f,  1.0f, 0.0f,    n_textures * 1.0f,             -1.0f   // Lower-right
};
// If a primitive (in this case vertex) has a color defined, then OpenGL will automatically
// create a gradient from one color to another. This is called interpolation.

// Index array for correctly going through vertices and avoiding duplicates:
//GLuint indices[] = {
//	0, 2, 1,  // Upper-left triangle
//	0, 3, 2,  // Lower-right triangle
//};
/*  _ _ _
 *  |   /|
 *  |  / |
 *  | /  |
 *  |/___|
 */
GLuint indices[] = {
	0, 2, 1,  // Lower-left triangle
	//0, 3, 2,  // Lower-right triangle
};

int main() {
	glfwInit();

	// Setting up glfw:
	// To let glfw know which OpenGL version we are using since it does not know otherwise:
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Check Note.md

	// 800x800 pixels:
	GLFWwindow* window = glfwCreateWindow(800, 800, "Texture", NULL, NULL);
	
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
	glViewport(0, 0, 800, 800);

	Shader shaderProgram("../shaders/texture.vert", "../shaders/texture.frag");

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
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Texture
	/*std::fstream f;

	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();*/

	Texture sadPepe("../data/sad_pepe.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	sadPepe.texUnit(shaderProgram, "tex0", 0);
	// OpenGl reads images from bottom-left->top-right, but stb reads images from top-left->bottom-right

	// Read "Textures" in Notes.md

	// Binding the VAO, VBO and EBO (Read Note.md for more info on "binding in OpenGL"):

	// Unbinding is first done for the VBO, then for VAO, and then for EBO (VERY IMPORTANT!!!)

	// Making sure that the window does not close instantly:
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);  // Background color Navy-blue (RGBA)
		glClear(GL_COLOR_BUFFER_BIT);			  // Cleaning back buffer and assigning it new color
		
		shaderProgram.Activate();
		// Uniforms can be assigned value only after activating the shader:
		glUniform1f(uniID, 0.5f);  // https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUniform.xhtml
		sadPepe.Bind();

		VAO1.Bind();

		// Drawing our object:
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);				  // To update graphics

		glfwPollEvents();  // Processes all pending events to prevent non-responsiveness
	}

	// Deleting objects to avoid memory leak:
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	sadPepe.Delete();
	shaderProgram.Delete();
	
	glfwDestroyWindow(window);
	glfwTerminate();  // Each initialize should be accompanied with a terminate

	return 0;
}