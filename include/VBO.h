// Lets C++ know not to open up file twice and avoids variable clashes:
#ifndef  VBO_CLASS_H
#define VBO_CLASS_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoords;
};

// Vertex Buffer Object class:
class VBO {

public:
	GLuint ID;

	/*Constructor take vertices in their size in bytes.
	Byte size of vertices is stored in "GLsizeiptr" datatype*/
	VBO(GLfloat* vertices, GLsizeiptr size);

	// Alternative:
	VBO(std::vector<Vertex>& vertices);

	void Bind();
	void Unbind();
	void Delete();
};

#endif