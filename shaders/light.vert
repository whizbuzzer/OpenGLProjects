#version 330 core					 // GLSL version. Using this version since we are using OpenGL 3.3.0

// Position coordinates
layout (location = 0) in vec3 aPos;  // Helps OpenGL read the vertex data it receives

uniform mat4 model;

uniform mat4 camMatrix;

void main()
{
	gl_Position = camMatrix * model * vec4(aPos, 1.0f);  // end element is 1
}