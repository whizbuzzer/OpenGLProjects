/* Definition of element Array Buffer Object class construtor and its methods */

#include "VAO.h"


VAO::VAO() {
	glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
	VBO.Bind();
	// Configuring VAO so that OpenGL knows how to read the VBO (Read Note.md!!!:
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	// To use the vertex attribute pointer, we need to enable it starting at 0:
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}
void VAO::Bind() {
	glBindVertexArray(ID);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &ID);
}