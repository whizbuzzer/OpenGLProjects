/* Model Class declaration header file */

#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <json/json.h>

#include "Mesh2.h"


// Creating alias
using json = nlohmann::json;

class Model {
public:
	// Loads in model from a file and stores that data in 'data', 'JSON' and 'fileName' attributes:
	Model(const char* fileName_);

	void Draw(Shader& shader, Camera& camera);  // No includes needed as shaderClass.h and Camera.h are already included in Mesh.h

private:
	const char* fileName;
	std::vector<unsigned char> data;  // Stores model points
	json JSON;

	// For avoiding storing of duplicate textures:
	std::vector<std::string> loadedTexNames;  // To determine whether or not we have loaded a texture
	std::vector<Texture2> loadedTextures;     // To store textures for all meshes to use

	// Meshes and their pose data:
	std::vector<Mesh2> meshes;
	std::vector<glm::vec3> meshTranslations;
	std::vector<glm::quat> meshRotations;  // Quaternions help avoid gimbal lock. a + bi + cj + dk
	std::vector<glm::vec3> meshScales;
	std::vector<glm::mat4> meshTransforms;  // Transformation matrix. Accounts for whether the former 3 properties would be used independently or together


	// Loading a single mesh by its index:
	void loadMesh(unsigned int meshIndex);

	// Traversing through a node and all its connecting nodes recursively to load mesh:
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	std::vector<unsigned char> getData();  // Gets us a vector of bytes from an external binary data file

	// Starting with the JSON tree leaves. Check out "Model Loading" in "Notes.md":
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture2> getTextures();

	// Combining attributes that we got from getFloats into a vertex:
	std::vector<Vertex> assembleVertices(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texCoords
	);

	// Array to vector converters:
	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};

#endif