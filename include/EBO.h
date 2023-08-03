// Lets C++ know not to open up file twice and avoids variable clashes:
#ifndef  EBO_CLASS_H
#define EBO_CLASS_H

#include <vector>

#include <glad/glad.h>


// Vertex Buffer Object class:
class EBO {

public:
	GLuint ID;

	EBO(GLuint* indices, GLsizeiptr size);

	EBO(std::vector<GLuint>& indices);

	void Bind();
	void Unbind();
	void Delete();
};

#endif