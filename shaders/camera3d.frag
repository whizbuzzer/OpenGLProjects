#version 330 core    // GLSL version

out vec4 FragColor;  // RGBA

in vec3 color;  // Inputs and outputs MUST have the same name

in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, vec2(texCoord.x, texCoord.y));
}