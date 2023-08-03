#version 330 core					 // GLSL version. Using this version since we are using OpenGL 3.3.0

// Position coordinates
layout (location = 0) in vec3 aPos;  // Helps OpenGL read the vertex data it receives

// Vertex colors
layout (location = 1) in vec3 aColor;

// Normals
layout(location = 2) in vec3 aNormal;

// Texture coordinates
layout(location = 3) in vec2 aTex;

// Providing colors for fragment shader since fragment shader takes care of colors:
out vec3 color;

// Providing normals to fragment shader:
out vec3 Normal;

// Providing texture coordinates to fragment shader:
out vec2 texCoord;

// Providing current position of the 3D mesh to the fragment shader.
// Will be used to calculate the direction of light:
out vec3 currPos;

// Camera pose matrix:
uniform mat4 camMatrix;

// Model pose matrix of a 3D mesh:
uniform mat4 model;

void main()
{
	currPos = vec3(model * vec4(aPos, 1.0f));

	// Outputs position coordinates to all vertices:
	gl_Position = camMatrix * vec4(aPos, 1.0);  // gl_Position is a keyword (Read Notes.md)
	
	// Assigns color from vertex data to "color" for fragment shader:
	color = aColor;

	// Assigns normal from vertex data to "Normal" for fragment shader:
	Normal = aNormal;

	// Assigns texture coordinates from vertex data to "texCoord" for fragment shader:
	texCoord = aTex;
}