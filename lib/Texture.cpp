/* Definitions of texture class and its methods */

#include "Texture.h"


Texture::Texture(const char* imagePath, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
	type = texType;
	int imgWidth, imgHeight, nColorChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(imagePath, &imgWidth, &imgHeight, &nColorChannels, 0);

	// Read "Textures" in Notes.md
	glGenTextures(1, &ID);
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// OpenGL has S(X), T(Y) and R(Z) axes. Since our image is 2D, setting for R axis is not needed:
	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// To use GL_CLAMP_TO_BORDER, you would also have to use glTexParameterfv():
	/*float flatColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);*/

	// Generating image texture:
	glTexImage2D(texType, 0, GL_RGBA, imgWidth, imgHeight, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);  // Mimap = smaller resolution versions of the image

	// Unloading image to free memory and unbinding texture:
	stbi_image_free(bytes);
	glBindTexture(texType, 0);
}

void Texture::texUnit(Shader shader, const char* uniform, GLuint unit) {
	GLuint tex0Uni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(tex0Uni, unit);
}

void Texture::Bind() {
	glBindTexture(type, ID);
}

void Texture::Unbind() {
	glBindTexture(type, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}