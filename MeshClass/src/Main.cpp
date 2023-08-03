/* Main file for Mesh class implementation */

#include "Mesh.h"


// Aspect ratio:
const unsigned int width = 800, height = 800;

/* To create a flat plane
 * Coordinates are normalized, so they gotta be in [-1, 1]
 * Using face normals/flat shading which is more suited for flatter meshes such as cubes and objects:
 */
Vertex vertices[] = {
		   // COORDINATES                 / COLORS                     / NORMALS                      / TEXTURE COORDS		  //
	Vertex{glm::vec3(-1.0f,  0.0f,  1.0f), glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 1.0f,  0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f,  0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 1.0f,  0.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3( 1.0f,  0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 1.0f,  0.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3( 1.0f,  0.0f,  1.0f), glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 1.0f,  0.0f), glm::vec2(1.0f, 0.0f)},
};  // It's okay if normals don't have unit length here as they will be normalized later in the shader

GLuint indices[] = {
	0, 1, 2,
	0, 2, 3
};

// Cube to serve as our light source:
Vertex lightVertices[] = {
	// COORDINATES	   //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},  // 0
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},  // 1
	Vertex{glm::vec3( 0.1f, -0.1f, -0.1f)},  // 2
	Vertex{glm::vec3( 0.1f, -0.1f,  0.1f)},  // 3
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},  // 4
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},  // 5
	Vertex{glm::vec3( 0.1f,  0.1f, -0.1f)},  // 6
	Vertex{glm::vec3( 0.1f,  0.1f,  0.1f)}	 // 7
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
	GLFWwindow* window = glfwCreateWindow(width, height, "MeshClass", NULL, NULL);

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

	Texture2 textures[]{
		Texture2("../data/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture2("../data/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

	/* Shader for 3D object */
	Shader shaderProgram("../shaders/mesh.vert", "../shaders/mesh.frag");

	// Generating Mesh for our object:
	std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector<GLuint> inds(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector<Texture2> texs(textures, textures + sizeof(textures) / sizeof(Texture2));
	Mesh floor(verts, inds, texs);

	/* Shader for light cube */
	Shader lightShader("../shaders/light.vert", "../shaders/light.frag");

	// Generating Mesh for our light cube:
	std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
	std::vector<GLuint> lightInds(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
	Mesh lightCube(lightVerts, lightInds, texs);  // Using same textures because light cube does ot have its own

	// Light cube
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);  // RBGA, normalized values.
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	// 3D object
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, objectPos);  // Why???

	// Activating shaders and gaining transformation matrices of individual objects:
	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	// We don't want light effects just from the light cube, but also have it reflect on the object:
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	// To tell OpenGL which surface to put on top of which and avoid glitches:
	glEnable(GL_DEPTH_TEST);

	// Camera object
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	// Making sure that the window does not close instantly:
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);  // Background color Navy-blue (RGBA)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			  // Cleaning back buffer and assigning it new color

		// Handles camera inputs that occur inside the window and updates camera pose accordingly:
		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		floor.Draw(shaderProgram, camera);
		lightCube.Draw(lightShader, camera);

		glfwSwapBuffers(window);				  // To update graphics

		glfwPollEvents();  // Processes all pending events to prevent non-responsiveness
	}

	// Deleting objects to avoid memory leak:
	shaderProgram.Delete();
	lightShader.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();  // Each initialize should be accompanied with a terminate

	return 0;
}