#version 330 core					 // GLSL version. Using this version since we are using OpenGL 3.3.0

// Position coordinates
layout (location = 0) in vec3 aPos;  // Helps OpenGL read the vertex data it receives

// Vertex colors
layout (location = 1) in vec3 aColor;

// Texture coordinates
layout(location = 2) in vec2 aTex;

// Providing colors for fragment shader since fragment shader takes care of colors:
out vec3 color;

// Providing texture coordinates to fragment shader:
out vec2 texCoord;

uniform mat4 camMatrix;

void main()
{
	gl_Position = camMatrix * vec4(aPos, 1.0);  // gl_Position is a keyword (Read Notes.md)
	
	color = aColor;

	texCoord = aTex;
}