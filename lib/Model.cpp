/* Model class definition file */

// #include <map>

#include "Model.h"


// Constructor
Model::Model(const char* fileName_) : fileName(fileName_) {
	std::string text = get_file_contents(fileName_);
	JSON = json::parse(text);

	data = getData();
}

// Methods:
std::vector<unsigned char> Model::getData() {
	std::string bytesText;  // Holds raw text
	// "uri" key gives us name of the .bin file which contains the binary data
	std::string uri = JSON["buffers"][0]["uri"];

	std::string fileNameStr = std::string(fileName);
	std::string fileDirectory = fileNameStr.substr(0, fileNameStr.find_last_of('/') + 1);

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
	unsigned int buffViewInd = accessor.value("bufferView", 1);    // Index to our bufferView of interest inside the complete data
	unsigned int accByteOffset = accessor.value("byteOffset", 0);  // accessor byte offset. Tells us from what index in the bufferView to look at the data w.r.t the beginning of the buffer

	// These two values always exist in GLTF files so no default/backup value required
	unsigned int count = accessor["count"];  // tells us how many floats we have to get from bufferView
	unsigned int componentType = accessor["componentType"];     // Tells us how we should group the floats SCALAR/VEC2/VEC3/VEC4. Look below!

	// creating JSON variable for bufferView dictionary so that we can get the byteOffset:
	json bufferView = JSON["bufferViews"][buffViewInd];
	unsigned int byteOffset = bufferView["byteOffset"];  // location to where the buffer starts after buffViewInd has located us the bufferView in the complete data

	unsigned int dataBeginning = byteOffset + accByteOffset;

	std::vector<GLuint> indicesVec;

	// 5125 = unsigned int
	// 5123 = unsigned short
	// 5122 = short
	if (componentType == 5125) {
		for (unsigned int i = dataBeginning; i < byteOffset + accByteOffset + count * 4; i) {
			unsigned char bytes[] = { data[i++], data[i++], data[i++], data[i++] };  // Incrementing here to get { data[1], data[2], data[3], data[4] }, etc.
			unsigned int value;
			std::memcpy(&value, bytes, sizeof(unsigned int));  // Transforms unsigned char array of bytes int float and stores them in the float value
			indicesVec.push_back((GLuint)value);
		}
	} else if (componentType == 5123) {
		for (unsigned int i = dataBeginning; i < byteOffset + accByteOffset + count * 2; i) {
			unsigned char bytes[] = { data[i++], data[i++] };
			unsigned short value;
			std::memcpy(&value, bytes, sizeof(unsigned short));
			indicesVec.push_back((GLuint)value);
		}
	} else if (componentType == 5122) {
		for (unsigned int i = dataBeginning; i < byteOffset + accByteOffset + count * 2; i) {
			unsigned char bytes[] = { data[i++], data[i++] };
			short value;
			std::memcpy(&value, bytes, sizeof(short));
			indicesVec.push_back((GLuint)value);
		}
	}

	return indicesVec;
}

// assembling vertices from gathered data:
std::vector<Vertex> Model::assembleVertices(
	std::vector<glm::vec3> positions,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> texCoords,
	) {
	std::vector<Vertex> vertices;

	for (int i = 0; i < positions.size(); i++) {
		vertices.push_back(Vertex{
				positions[i],
				normals[i],
				glm::vec3(1.0f, 1.0f, 1.0f),  // Color. Not custom declared as vertex colors are uncommon in 3D models.
				texCoords[i]
			}
		);

		return vertices;
	}
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