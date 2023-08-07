/* Mesh class definition
 * UPDATE: Draw function rehauled. Now contains mesh pose data as arguments.
 */

#ifndef MESH2_CLASS_H
#define MESH2_CLASS_H

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture2.h"

class Mesh2 {
public:
	// All properties initialized as vectors for flexibility. <vector> has already been included in VBO.h:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture2> textures;

	VAO VAO;

	Mesh2(std::vector<Vertex>& vertices_, std::vector<GLuint>& indices_, std::vector<Texture2>& textures_);

	void Draw(
		Shader& shader,
		Camera& camera,
		glm::mat4 meshTransform = glm::mat4(1.0f),
		glm::vec3 meshTranslation = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::quat meshRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec3 meshScale = glm::vec3(1.0f, 1.0f, 1.0f)
	);
};

#endif // !MESH_CLASS_H
