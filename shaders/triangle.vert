#version 330 core					 // GLSL version. Using this version since we are using OpenGL 3.3.0

layout (location = 0) in vec3 aPos;  // Helps OpenGL read the vertex data it receives

layout (location = 1) in vec3 aColor;

// Providing colors for fragment shader since fragment shader takes care of colors:
out vec3 color;

uniform float scale;  // NEVER declare uniforms if you are not going to use them

void main()
{
	gl_Position = vec4(
		aPos.x + aPos.x * scale,
		aPos.y + aPos.y * scale,
		aPos.z + aPos.z * scale,
		1.0
	);  // gl_Position is a keyword (Read Notes.md)
	
	color = aColor;
}