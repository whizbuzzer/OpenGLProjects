#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.h"

class Camera {

public:
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);  // Rotation angles in radians
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);			   // To move along Y-axis
	glm::mat4 cameraMatrix = glm::mat4(1.0f);			   // Camera transformation matrix initialized as identity matrix

	// Window width and height:
	int width;
	int height;
	
	// Camera movement properties:
	float speed = 0.1f;		// Translation
	float sensitivity = 100.0f;  // Rotation

	bool firstClick = true;   // To stop cursor from jumping
	float invertAxes = 1.0f;  // To invert camera axes
	bool iPressed = false;	  // For toggling "invert axes"

	Camera(int width_, int height_, glm::vec3 position);

	// Exports the camera matrix to a shader.
	// Only objects that fall withing this range will be rendered:
	void Matrix(Shader& shader, const char* uniform);

	// Updates camera matrix to the vertex shader.
	// nearPlane and farPlane dictate the rendering range of our program:
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);

	void Inputs(GLFWwindow* window);  // For moving the camera
};

#endif