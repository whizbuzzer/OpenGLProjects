/* Texture class 2.0 texType is no longer an enum, but a character array pointer
 * UPDATE: removed format and pixel type as arguments as they are no longer needed
 */
#ifndef TEXTURE2_CLASS_H
#define TEXTURE2_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "shaderClass.h"


class Texture2 {

public:
	GLuint ID;
	const char* type;
	GLuint unit;  // For accomodating multiple textures. Textures are stored in texture units

	Texture2(const char* imagePath, const char* texType, GLenum slot);

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();
};

#endif