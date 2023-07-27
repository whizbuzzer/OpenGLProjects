// Lets C++ know not to open up file twice and avoids variable clashes:
#ifndef  VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>

#include "VBO.h"


// Vertex Buffer Object class:
class VAO {

public:
	GLuint ID;

	VAO();

	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);  // For shaders
	void Bind();
	void Unbind();
	void Delete();
};

#endif