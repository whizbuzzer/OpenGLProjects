/* Main file for Mesh class implementation */

#include "Model.h"


// Aspect ratio:
const unsigned int width = 800, height = 800;


int main() {
	glfwInit();

	// Setting up glfw:
	// To let glfw know which OpenGL version we are using since it does not know otherwise:
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Check Note.md

	// width x height pixels:
	GLFWwindow* window = glfwCreateWindow(width, height, "ModelLoading", NULL, NULL);

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
	Shader shaderProgram("../shaders/model.vert", "../shaders/model.frag");

	// Light cube
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);  // RBGA, normalized values.
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// To tell OpenGL which surface to put on top of which and avoid glitches:
	glEnable(GL_DEPTH_TEST);

	// Camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	// Loading the model:
	Model model("../data/models/sword/scene.gltf");

	// Making sure that the window does not close instantly:
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);  // Background color Navy-blue (RGBA)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			  // Cleaning back buffer and assigning it new color

		// Handles camera inputs that occur inside the window and updates camera pose accordingly:
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		model.Draw(shaderProgram, camera);

		glfwSwapBuffers(window);				  // To update graphics

		glfwPollEvents();  // Processes all pending events to prevent non-responsiveness
	}

	// Deleting objects to avoid memory leak:
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();  // Each initialize should be accompanied with a terminate

	return 0;
}