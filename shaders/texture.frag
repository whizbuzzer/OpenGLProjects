#version 330 core    // GLSL version

out vec4 FragColor;

in vec3 color;  // Inputs and outputs MUST have the same name

in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
	// FragColor = vec4(color, 1.0f);  // RGBA
	FragColor = texture(tex0, vec2(texCoord.x, texCoord.y));
	// FragColor = texture(tex0, vec2(-texCoord.x, texCoord.y));  // For mirroring on vertical axis
}