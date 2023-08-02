#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "shaderClass.h"


class Texture {

public:
	GLuint ID;
	GLenum type;
	GLuint unit;  // For accomodating multiple textures. Textures are stored in texture units

	Texture(const char* imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};

#endif