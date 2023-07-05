#version 330 core    // GLSL version

out vec4 FragColor;

in vec3 color;  // Inputs and outputs MUST have the same name

void main()
{
	FragColor = vec4(color, 1.0f);  // RGBA
}