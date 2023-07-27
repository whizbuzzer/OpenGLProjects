// Lets C++ know not to open up file twice and avoids variable clashes:
#ifndef  VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>


// Vertex Buffer Object class:
class VBO {

public:
	GLuint ID;

	/*Constructor take vertices in their size in bytes.
	Byte size of vertices is stored in "GLsizeiptr" datatype*/
	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};

#endif