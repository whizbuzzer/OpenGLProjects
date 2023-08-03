/* Definition of Element Array Buffer Object class construtor and its methods */

#include "EBO.h"


EBO::EBO(GLuint* indices, GLsizeiptr size) {
	glGenBuffers(1, &ID);  // 1 because only 1 VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);  // Last argument is usage. Read "Note.md"!!!
}

EBO::EBO(std::vector<GLuint>& indices) {
	glGenBuffers(1, &ID);  // 1 because only 1 VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);  // Last argument is usage. Read "Note.md"!!!
}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
	glDeleteBuffers(1, &ID);
}