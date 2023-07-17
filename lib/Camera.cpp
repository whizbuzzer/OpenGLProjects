/* Definitions of Camera class and its methods */

#include "Camera.h"

Camera::Camera(int width_, int height_, glm::vec3 position) : width(width_), height(height_), Position(position) {}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform) {
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

	// Exporting matrix to vertex shader:
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Camera* camera =
		static_cast<Camera*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
		invertAxes = -1;
}

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
		Position += speed * glm::normalize(glm::cross(Orientation, Up));;
	}
	// Up
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		Position += speed * Up;
	}
	// Down
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		Position += speed * -Up;
	}

	// Invert camera axes:
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		if (!iPressed)
			invertAxes *= -1;
		iPressed = true;
	} else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE) { // Move slow
		iPressed = false;
	}  // Gotta figure out a key toggle option

	// Move fast
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		speed = 0.4f;
	} else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) { // Move slow
		speed = 0.01f;
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

		// Rotate about Y-axis:
		Orientation = glm::rotate(Orientation, glm::radians(invertAxes * rotY), Up);

		// Sets cursor to center of screen to prevent it from roaming around:
		glfwSetCursorPos(window, (width / 2), (height / 2));
	} else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}