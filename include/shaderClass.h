// Lets C++ know not to open up file twice and avoids variable clashes:
#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <cerrno>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include <glad/glad.h>

std::string get_file_contents(const char* filename);

// Shader class:
class Shader {

public:
	GLuint ID;

	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();

private:
	// Error-checking:
	void compileErrors(unsigned int shader, const char* type);
};

#endif