#version 330 core    // GLSL version

out vec4 FragColor;  // RGBA

in vec3 color;  // Inputs and outputs MUST have the same name

in vec2 texCoord;

in vec3 Normal;

in vec3 currPos;

uniform sampler2D tex0;

uniform vec4 lightColor;

uniform vec3 lightPos;

uniform vec3 camPos;

void main()
{
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - currPos);

	// We will be implementing diffuse lighting (Refer to "Lighting" in Notes.md for reasoning behind dot product!!!):
	float diffuse = max(dot(normal, lightDirection), 0.0f);  // ReLU because we don't want negative color value

	// Ambient lighting to simulate light scattering simply and avoid pitch-black surfaces which do not face the light source:
	float ambient = 0.20f;   // Arbitrary value

	// Specular lighting to simulate reflective properties of the surfaces:
	float specularLight = 0.50f;  // Arbitrary value

	vec3 viewDirection = normalize(camPos - currPos);

	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/reflect.xhtml
	vec3 reflectionDirection = reflect(-lightDirection, normal);  // negative lightDirection because we want reflectionDirection to be towards the plane, not away from it.

	// To store how much specular light we have at a certain angle.
	// Specular light intensity is inversely proportional to the angle between viewDirection and reflectionDirection:
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);  // Definition and point-likeness of specular light is directly proportional to the power
	float specular = specularLight * specAmount;

	FragColor = texture(tex0, vec2(texCoord.x, texCoord.y)) * lightColor * (diffuse + ambient + specular);
}