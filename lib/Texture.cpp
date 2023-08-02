/* Definitions of texture class and its methods */

#include "Texture.h"


Texture::Texture(const char* imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) : type(texType) {
	//type = texType;
	int imgWidth, imgHeight, nColorChannels;
	stbi_set_flip_vertically_on_load(true);  // Flips the image so that it appears right side up
	unsigned char* bytes = stbi_load(imagePath, &imgWidth, &imgHeight, &nColorChannels, 0);

	// Read "Textures" in Notes.md
	glGenTextures(1, &ID);				  // Texture generation
	glActiveTexture(GL_TEXTURE0 + slot);  // Assigning texture to texture unit
	unit = slot;
	glBindTexture(texType, ID);

	// Configuring algorithm type to make image smaller/bigger
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// OpenGL has S(X), T(Y) and R(Z) axes. Since our image is 2D, setting for R axis is not needed:
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);


	// Generating image texture:
	glTexImage2D(texType, 0, GL_RGBA, imgWidth, imgHeight, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);  // Mimap = smaller resolution versions of the image

	// Unloading image to free memory and unbinding texture
	// as image has already been assigned to OpenGL texture object:
	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) {
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind() {
	glActiveTexture(GL_TEXTURE0 + unit);  // Almost lost my mind due to this line's absence. Req.d for accomodating multiple textures
	glBindTexture(type, ID);
}

void Texture::Unbind() {
	glBindTexture(type, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}