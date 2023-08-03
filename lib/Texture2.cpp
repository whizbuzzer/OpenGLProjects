/* Definitions of texture class and its methods */

#include "Texture2.h"


Texture2::Texture2(const char* imagePath, const char* texType, GLenum slot, GLenum format, GLenum pixelType) : type(texType) {
	// type = texType;
	// type_str = textTypeStr;
	int imgWidth, imgHeight, nColorChannels;
	stbi_set_flip_vertically_on_load(true);  // Flips the image so that it appears right side up
	unsigned char* bytes = stbi_load(imagePath, &imgWidth, &imgHeight, &nColorChannels, 0);

	// Read "Textures" in Notes.md
	glGenTextures(1, &ID);				  // Texture generation
	glActiveTexture(GL_TEXTURE0 + slot);  // Assigning texture to texture unit
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Configuring algorithm type to make image smaller/bigger
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// OpenGL has S(X), T(Y) and R(Z) axes. Since our image is 2D, setting for R axis is not needed:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	// Generating image texture:
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, format, pixelType, bytes);
	glGenerateMipmap(GL_TEXTURE_2D);  // Mimap = smaller resolution versions of the image

	// Unloading image to free memory and unbinding texture
	// as image has already been assigned to OpenGL texture object:
	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2::texUnit(Shader& shader, const char* uniform, GLuint unit) {
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture2::Bind() {
	glActiveTexture(GL_TEXTURE0 + unit);  // Almost lost my mind due to this line's absence. Req.d for accomodating multiple textures
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture2::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2::Delete() {
	glDeleteTextures(1, &ID);
}