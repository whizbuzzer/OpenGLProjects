/* Definitions of Camera class and its methods */

#include "Camera.h"

Camera::Camera(int width_, int height_, glm::vec3 position) : width(width_), height(height_), Position(position) {}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look at the right direction from the right position
	view = glm::lookAt(Position, Position + Orientation, Up);
	// Dictates how much of the graphics would appear in the visualizer window
	projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::Matrix(Shader& shader, const char* uniform) {
	// Exporting matrix to vertex shader:
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

// Gotta find how to print which key was pressed on the visualizer
void Camera::Inputs(GLFWwindow* window) {
	/* Key Inputs */
	// Front
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		Position += speed * Orientation;
	}
	// Back
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		Position += speed * -Orientation;
	}
	// Left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	// Right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	// Up
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		Position += speed * Up;
	}
	// Down
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		Position += speed * -Up;
	}

	// Gotta still fix rotating
	// Rotating about Y clockwise:
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		// Rotate about Y-axis (left and right):
		Orientation = glm::rotate(Orientation, glm::radians(invertAxes * 0.01f), Up);
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}

	// Rotating about Y counter-clockwise:
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		// Rotate about Y-axis (left and right):
		Orientation = glm::rotate(Orientation, glm::radians(invertAxes * -0.01f), Up);
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}

	// Invert camera axes:
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		if (!iPressed) {
			invertAxes *= -1.0f;
			iPressed = true;
		}
	} else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE) {
		iPressed = false;
	}

	// Move speed adjustment
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {  // Move fast
		speed = 0.01f;
	} else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {  // Move slow
		speed = 0.001f;
	}  // Gotta figure out a key toggle option

	/* Mouse Inputs */
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		if (firstClick) {
			// Sets cursor to center of screen to prevent it from roaming around:
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Mouse position:
		double mouseX;
		double mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Rotation of orientation vector. Modified and normalised to account for cursor coordinates originating at top-left corner of screen:
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Rotate about X-axis:
		// Orientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(invertAxes * rotX), glm::normalize(glm::cross(Orientation, Up)));

		// Prevents barrel-rolling about X-axis:
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
			Orientation = newOrientation;
		}

		// Rotate about Y-axis (left and right):
		Orientation = glm::rotate(Orientation, glm::radians(invertAxes * rotY), Up);

		// Sets cursor to center of screen to prevent it from roaming around:
		glfwSetCursorPos(window, (width / 2), (height / 2));
	} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}