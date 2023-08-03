#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture2.h"

class Mesh {
public:
	// All properties initialized as vectors for flexibility. <vector> has already been included in VBO.h:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture2> textures;

	VAO VAO;

	Mesh(std::vector<Vertex>& vertices_, std::vector<GLuint>& indices_, std::vector<Texture2>& textures_);

	void Draw(Shader& shader, Camera& camera);
};

#endif // !MESH_CLASS_H
