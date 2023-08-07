/* Model class definition file */

// #include <map>

#include "Model.h"


// Constructor
Model::Model(const char* fileName_) : fileName(fileName_) {
	std::string text = get_file_contents(fileName);
	JSON = json::parse(text);

	data = getData();

	traverseNode(0);  // Second argument not needed as it has a default value
}

// Methods:
void Model::Draw(Shader& shader, Camera& camera) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Mesh2::Draw(shader, camera, meshTransforms[i]);
	}
}

void Model::loadMesh(unsigned int meshIndex) {
	// Getting indices of all vertex accessors:
	unsigned int posAccInd = JSON["meshes"][meshIndex]["primitives"][0]["attributes"]["POSITION"];
	unsigned int normalAccInd = JSON["meshes"][meshIndex]["primitives"][0]["attributes"]["NORMAL"];
	unsigned int texAccInd = JSON["meshes"][meshIndex]["primitives"][0]["attributes"]["TEXCOORD_0"];
	unsigned int indAccInd = JSON["meshes"][meshIndex]["primitives"][0]["indices"];

	// Preparing vertices components:
	std::vector<float> posVec = getFloats(JSON["accessors"][posAccInd]);
	std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
	std::vector<float> normalVec = getFloats(JSON["accessors"][normalAccInd]);
	std::vector<glm::vec3> normals = groupFloatsVec3(normalVec);
	std::vector<float> texVec = getFloats(JSON["accessors"][texAccInd]);
	std::vector<glm::vec2> texCoords = groupFloatsVec2(texVec);

	// Preparing arguments to create meshes:
	std::vector<Vertex> vertices = assembleVertices(positions, normals, texCoords);
	std::vector<GLuint> indices = getIndices(JSON["accessors"][indAccInd]);
	std::vector<Texture2> textures = getTextures();

	// Creating and loading meshes:
	meshes.push_back(Mesh2(vertices, indices, textures));
}

void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix) {
	// Current node
	json node = JSON["nodes"][nextNode];

	// Extracting our matrices if they exist:
	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
	if (node.find("translation") != node.end()) {  // Similar method to set/map!
		float translValues[3];
		for (unsigned int i = 0; i < node["translation"].size(); i++)
			translValues[i] = (node["translation"][i]);
		translation = glm::make_vec3(translValues);
	}

	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (node.find("rotation") != node.end()) {  // Similar method to set/map!
		float rotValues[4] = {  // Order is different because GLM forms quaternions as W(3)->X(0)->Y(1)->Z(2)
			node["rotation"][3],
			node["rotation"][0],
			node["rotation"][1],
			node["rotation"][2]
		};
		rotation = glm::make_quat(rotValues);
	}

	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	if (node.find("scale") != node.end()) {  // Similar method to set/map!
		float scaleValues[3];
		for (unsigned int i = 0; i < node["scale"].size(); i++)
			scaleValues[i] = (node["scale"][i]);
		scale = glm::make_vec3(scaleValues);
	}

	glm::mat4 model = glm::mat4(1.0f);
	if (node.find("matrix") != node.end()) {  // Similar method to set/map!
		float modelValues[16];  // 4x4
		for (unsigned int i = 0; i < node["matrix"].size(); i++)
			modelValues[i] = (node["matrix"][i]);
		model = glm::make_mat4(modelValues);
	}

	// Preparing and combining all transformations for the next node:
	glm::mat4 translMat = glm::mat4(1.0f);
	glm::mat4 rotMat = glm::mat4(1.0f);
	glm::mat4 scaleMat = glm::mat4(1.0f);

	translMat = glm::translate(translMat, translation);
	rotMat = glm::mat4_cast(rotation);  // To convert from quaternion to 4x4
	scaleMat = glm::scale(scale);

	glm::mat4 nextNodeMatrix = matrix * model * translMat * rotMat * scaleMat;

	// Checking if a node contains a mesh, and then loading it accordingly:
	if (node.find("mesh") != node.end()) {
		meshTranslations.push_back(translation);
		meshRotations.push_back(rotation);
		meshScales.push_back(scale);
		meshTransforms.push_back(nextNodeMatrix);

		loadMesh(node["mesh"]);
	}

	if (node.find("children") != node.end()) {
		for (unsigned int i = 0; i < node["children"].size(); i++) {
			traverseNode(node["children"][i], nextNodeMatrix);
		}

		/*for (auto child:node["children"]) {
			traverseNode(child, nextNodeMatrix);
		}*/
	}
}

std::vector<unsigned char> Model::getData() {
	std::string bytesText;  // Holds raw text
	// "uri" key gives us name of the .bin file which contains the binary data
	std::string uri = JSON["buffers"][0]["uri"];

	std::string fileNameStr = std::string(fileName);
	std::string fileDirectory = fileNameStr.substr(0, fileNameStr.find_last_of('/') + 1);  // Everything but the file name

	// Getting the bytes data:
	bytesText = get_file_contents((fileDirectory + uri).c_str());
	// Constructing vector from string:
	std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
	return data;
}

std::vector<float> Model::getFloats(json accessor) {
	// ".value() function will give you the value of the key if it exits or gives us the default value that we have set instead. Prevents KeyError
	// These two values need defaults as they do not always exist in GLTF files
	unsigned int buffViewInd = accessor.value("bufferView", 1);    // Index to our bufferView of interest inside the complete data
	unsigned int accByteOffset = accessor.value("byteOffset", 0);  // accessor byte offset. Tells us from what index in the bufferView to look at the data w.r.t the beginning of the buffer

	// These two values always exist in GLTF files so no default/backup value required
	unsigned int count = accessor["count"];  // tells us how many floats we have to get from bufferView
	std::string type = accessor["type"];     // Tells us how we should group the floats SCALAR/VEC2/VEC3/VEC4. Look below!

	// creating JSON variable for bufferView dictionary so that we can get the byteOffset:
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];  // location to where the buffer starts after buffViewInd has located us the bufferView in the complete data
	
	/* We have 2 different offsets because 2 different accessors may share a bufferView (e.g. position accessor and normal accessor may share the same bufferView since both datatypes are of the same size*/

	unsigned int nFloatsPerVert;  // Number of floats per vertex based on "type"

	if (type == "SCALAR")
		nFloatsPerVert = 1;
	else if (type == "VEC2")
		nFloatsPerVert = 2;
	else if (type == "VEC3")
		nFloatsPerVert = 3;
	else if (type == "VEC4")
		nFloatsPerVert = 4;
	else
		throw std::invalid_argument("Type is invalid. Should be SCALAR, VEC2, VEC3 or VEC4");

	/*
	// Does not work due to std::string vs const char*. Could be solved with mapping those const char* to and enum or a map: 
	switch (type) {
		case "SCALAR":
			nFloatsPerVert = 1;
			break;
		case "VEC2":
			nFloatsPerVert = 2;
			break;
		case "VEC3":
			nFloatsPerVert = 3;
			break;
		case "VEC4":
			nFloatsPerVert = 4;
			break;
		default:
			throw std::invalid_argument("Type is invalid. Should be SCALAR, VEC2, VEC3 or VEC4");
			break;
	}*/

	unsigned int dataBeginning = byteOffset + accByteOffset;
	unsigned int dataLength = count * 4 * nFloatsPerVert;  // 4 because each vertex has 4 attributes: coordinates, color, normal and texCoords???


	std::vector<float> floatVec;

	for (unsigned int i = dataBeginning; i < dataBeginning + dataLength; i) {
		unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };  // Incrementing here to get { data[1], data[2], data[3], data[4] }, etc.
		float value;
		std::memcpy(&value, bytes, sizeof(float));  // Transforms unsigned char array of bytes int float and stores them in the float value
		floatVec.push_back(value);
	}

	return floatVec;
}

std::vector<GLuint> Model::getIndices(json accessor) {
	unsigned int buffViewInd = accessor.value("bufferView", 0);    // Index to our bufferView of interest inside the complete data
	unsigned int accByteOffset = accessor.value("byteOffset", 0);  // accessor byte offset. Tells us from what index in the bufferView to look at the data w.r.t the beginning of the buffer

	// These two values always exist in GLTF files so no default/backup value required
	unsigned int count = accessor["count"];  // tells us how many floats we have to get from bufferView
	unsigned int componentType = accessor["componentType"];     // Tells us how we should group the floats SCALAR/VEC2/VEC3/VEC4. Look below!

	// creating JSON variable for bufferView dictionary so that we can get the byteOffset:
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];  // location to where the buffer starts after buffViewInd has located us the bufferView in the complete data

	unsigned int dataBeginning = byteOffset + accByteOffset;

	std::vector<GLuint> indices;

	// 5125 = unsigned int
	// 5123 = unsigned short
	// 5122 = short
	if (componentType == 5125) {
		for (unsigned int i = dataBeginning; i < byteOffset + accByteOffset + count * 4; i) {
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };  // Incrementing here to get { data[1], data[2], data[3], data[4] }, etc.
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));  // Transforms unsigned char array of bytes int float and stores them in the float value
			indices.push_back((GLuint)value);
		}
	} else if (componentType == 5123) {
		for (unsigned int i = dataBeginning; i < byteOffset + accByteOffset + count * 2; i) {
			unsigned char bytes[] = { data[i++], data[i++] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indices.push_back((GLuint)value);
		}
	} else if (componentType == 5122) {
		for (unsigned int i = dataBeginning; i < byteOffset + accByteOffset + count * 2; i) {
			unsigned char bytes[] = { data[i++], data[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indices.push_back((GLuint)value);
		}
	}

	return indices;
}

std::vector<Texture2> Model::getTextures() {
	std::string fileStr = std::string(fileName);
	std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);  // To get everything but the file name

	std::vector<Texture2> textures;

	for (unsigned int i = 0; i < JSON["images"].size(); i++) {
		// Directly accessing the "images" key to get an array of Uniform Resource Identifiers (URI):
		std::string texturePath = JSON["images"][i]["uri"];

		// Skipping textures which have the same name as a texture among loaded textures i.e. a texture that has already been loaded:
		bool skip = false;
		for (unsigned int j = 0; j < loadedTexNames.size(); j++) {
			if (loadedTexNames[j] == texturePath) {
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}


		if (!skip) {
			// Checking for diffuse/specular maps:
			// npos is a const static member of string datatype.
			// It indicates highest possible value for element of type size_t i.e. end of string
			// Return value of .find() != npos means that a certain string exists.
			if (texturePath.find("baseColor") != std::string::npos) {  // baseColor string exists -> Diffuse map found!
				Texture2 diffuse = Texture2((fileDirectory + texturePath).c_str(), "diffuse", loadedTextures.size());
				textures.push_back(diffuse);
				loadedTextures.push_back(diffuse);
				loadedTexNames.push_back(texturePath);
			}
			else if (texturePath.find("metallicRoughness") != std::string::npos) {  // Specular map found!
				Texture2 specular = Texture2((fileDirectory + texturePath).c_str(), "specular", loadedTextures.size());
				textures.push_back(specular);
				loadedTextures.push_back(specular);
				loadedTexNames.push_back(texturePath);
			}
		}
	}

	return textures;
}

// assembling vertices from gathered data:
std::vector<Vertex> Model::assembleVertices(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texCoords
	) {
	std::vector<Vertex> vertices;

	for (int i = 0; i < positions.size(); i++) {
		vertices.push_back(
			Vertex{
				positions[i],
				normals[i],
				glm::vec3(1.0f, 1.0f, 1.0f),  // Color. Not custom declared as vertex colors are uncommon in 3D models.
				texCoords[i]
			}
		);
	}

	return vertices;
}

// Array to vector converters:
std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec) {
	std::vector<glm::vec2> vectors;

	for (int i = 0; i < floatVec.size(); i) {
		vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
	}

	return vectors;
}

std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec) {
	std::vector<glm::vec3> vectors;

	for (int i = 0; i < floatVec.size(); i) {
		vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
	}

	return vectors;
}

std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec) {
	std::vector<glm::vec4> vectors;

	for (int i = 0; i < floatVec.size(); i) {
		vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
	}

	return vectors;
}