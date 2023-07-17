#pragma once

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
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);  // To move along Y-axis

	int width;
	int height;
	
	float speed = 0.01f;  // Camera movement speed
	float sensitivity = 10.0f;

	bool firstClick = true;   // To stop cursor from jumping
	float invertAxes = 1.0f;  // To invert camera axes
	bool iPressed = false;

	Camera(int width_, int height_, glm::vec3 position);

	// nearPlane and farPlane dictate the rendering range of our program. Only objects that fall withing this range will be rendered:
	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
	void Inputs(GLFWwindow* window);  // For moving the camera!
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif