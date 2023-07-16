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

uniform float scale;  // NEVER declare uniforms if you are not going to use them

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	gl_Position = proj * view * model * vec4(
		aPos.x + aPos.x * scale,
		aPos.y + aPos.y * scale,
		aPos.z + aPos.z * scale,
		1.0
	);  // gl_Position is a keyword (Read Notes.md)
	
	color = aColor;

	texCoord = aTex;
}