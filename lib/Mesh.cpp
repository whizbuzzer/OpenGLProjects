/* Mesh class definition file */

#include "Mesh.h"


// Constructor:
Mesh::Mesh(std::vector<Vertex>& vertices_,
	std::vector<GLuint>& indices_,
	std::vector<Texture2>& textures_) : vertices(vertices_), indices(indices_), textures(textures_) {
	VAO.Bind();

	VBO VBO(vertices);

	EBO EBO(indices);

	// Linking vertex attributes:
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);					  // Positions
	VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));  // Colors
	VAO.LinkAttrib(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));  // Normals
	VAO.LinkAttrib(VBO, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));  // Texture coordinates

	// To prevent accidental modification:
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

// Methods:
void Mesh::Draw(Shader& shader, Camera& camera) {
	shader.Activate();
	VAO.Bind();

	// Keeping count for diffuse and specular maps:
	unsigned int numDiffuse = 0, numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++) {
		std::string num;
		std::string type = textures[i].type;

		if (type == "diffuse") {
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular") {
			num = std::to_string(numSpecular++);
		}

		textures[i].texUnit(shader, (type + num).c_str(), i);
		textures[i].Bind();
	}

	// Exports camera position to fragment shader for specular lighting:
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

	// Exports camera matrix to vertex shader
	camera.Matrix(shader, "camMatrix");

	// Drawing our object:
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}