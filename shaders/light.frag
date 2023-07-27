#version 330 core    // GLSL version

out vec4 FragColor;  // RGBA

uniform vec4 lightColor;

void main()
{
	// FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	FragColor = lightColor;
}