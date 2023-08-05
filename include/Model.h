/* Model Class declaration header file */

#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <json/json.h>

#include "Mesh.h"


// Creating alias
using json = nlohmann::json;

class Model {
public:
	Model(const char* fileName_);

	void Draw(Shader& shader, Camera& camera);  // No includes needed as shaderClass.h and Camera.h are already included in Mesh.h

private:
	const char* fileName;
	std::vector<unsigned char> data;  // Stores model points
	json JSON;

	// Meshes and their transformations:
	std::vector<Mesh> meshes;
	std::vector<glm::vec3> meshTranslations;
	std::vector<glm::quat> meshRotations;
	std::vector<glm::vec3> meshScales;
	std::vector<glm::mat4> meshTransformations;

	// Ensuring textures get loaded only once:
	std::vector<std::string> loadedTexNames;
	std::vector<Texture2> loadedTextures;

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