/* Definition of Shader class construtor and its methods */

#include "shaderClass.h"


// File reader function:
std::string get_file_contents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Shader constructor:
Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	// Acquiring source codes:
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Storing source codes into character arrays:
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Initializing vertex shader:
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);  // 1 screen for the whole shader
	// GPU can't understand the source code so it has to be compiled immediately!:
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	// Initializing fragment shader:
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	// To use shaders, you have to wrap them up into a "shader program":
	ID = glCreateProgram();  // shaderProgram = glCreateProgram();

	for (auto shader : { vertexShader, fragmentShader })
		glAttachShader(ID, shader);
	/*glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);*/

	glLinkProgram(ID);  // Wrapping off shader program

	// Ensuring that shaders have linked successfully:
	compileErrors(ID, "PROGRAM");

	// Deleting shader as they are already linked to the shader program and no longer needed
	// (freeing memory):
	for (auto shader : { vertexShader, fragmentShader })
		glDeleteShader(shader);
	/*glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);*/
}

void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteShader(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type) {
	// Compilation status:
	GLint hasCompiled;

	// Error message storage:
	char infoLog[1024];

	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);

		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		} else {
			glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);

			if (hasCompiled == GL_FALSE) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
			}
		}
	}
}