#version 330 core    // GLSL version

out vec4 FragColor;  // RGBA


in vec3 currPos;

in vec3 Normal;

in vec3 color;  // Inputs and outputs MUST have the same name

in vec2 texCoord;


uniform sampler2D diffuse0;  // For texture/diffuse map

uniform sampler2D specular0;  // For specular map

uniform vec4 lightColor;

uniform vec3 lightPos;

uniform vec3 camPos;


vec4 pointLight() {
	// Distance between light source and current position of 3D mesh:
	vec3 lightVec = lightPos - currPos;
	float dist = length(lightVec);

	// Arbitrary constants for out light equation (check "Types of Light Sources" in "Notes.md"):
	float a = 3.0;  // Quadratic term. Seems to change light intensity nearest to the surface
	float b = 0.7;  // Linear term. Seems to change intensity of darkness away from light

	// Light intensity calculation using out equation:
	float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// Ambient lighting to simulate light scattering simply and avoid pitch-black surfaces which do not face the light source:
	float ambient = 0.20f;   // Arbitrary value

	// We will be implementing diffuse lighting (Refer to "Lighting" in Notes.md for reasoning behind dot product!!!):
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);  // ReLU because we don't want negative color value

	// Specular lighting to simulate reflective properties of the surfaces:
	float specularLight = 0.50f;  // Arbitrary value
	
	// To store how much specular light we have at a certain angle.
	// Specular light intensity is inversely proportional to the angle between viewDirection and reflectionDirection:
	vec3 viewDirection = normalize(camPos - currPos);

	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/reflect.xhtml
	vec3 reflectionDirection = reflect(-lightDirection, normal);  // negative lightDirection because we want reflectionDirection to be towards the plane, not away from it.

	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);  // Definition and point-likeness of specular light is directly proportional to the power
	float specular = specularLight * specAmount;
	
	// Light intensity is applied only to diffuse and specular component to manipulate base colors and reflectivity:
	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + texture(specular0, texCoord).r * specular * intensity) * lightColor;  
}

vec4 directionalLight() {
	// Ambient lighting to simulate light scattering simply and avoid pitch-black surfaces which do not face the light source:
	float ambient = 0.20f;   // Arbitrary value

	// We will be implementing diffuse lighting (Refer to "Lighting" in Notes.md for reasoning behind dot product!!!):
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 1.0f));
	float diffuse = max(dot(normal, lightDirection), 0.0f);  // ReLU because we don't want negative color value

	// Specular lighting to simulate reflective properties of the surfaces:
	float specularLight = 0.50f;  // Arbitrary value
	
	// To store how much specular light we have at a certain angle.
	// Specular light intensity is inversely proportional to the angle between viewDirection and reflectionDirection:
	vec3 viewDirection = normalize(camPos - currPos);

	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/reflect.xhtml
	vec3 reflectionDirection = reflect(-lightDirection, normal);  // negative lightDirection because we want reflectionDirection to be towards the plane, not away from it.

	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);  // Definition and point-likeness of specular light is directly proportional to the power
	float specular = specularLight * specAmount;
	
	// Light intensity is applied only to diffuse and specular component to manipulate base colors and reflectivity:
	return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;  // Specular map has only the red channel here. Therefore ".r" used.
}

vec4 spotLight() {
	// Angle cosines. Using only one of them makes the lighting look like a cut out, but using both of them gives a nice gradient to the lighting:
	float innerCone = 0.95f;  // Cosine of angle between light direction and inner cone circumference. Represent central light
	float outerCone = 0.90f;  // Cosine of angle between light direction and outer cone circumference. Represents darkness near the light cone boundary
	
	// Cosines lie between [0, 1] and therefore are computationally cheaper than using the angles directly
	// innerCone value needs to be larger tha outerCone value otherwise the ligh source will cast a shadow instead of a light lol

	// Ambient lighting to simulate light scattering simply and avoid pitch-black surfaces which do not face the light source:
	float ambient = 0.20f;   // Arbitrary value

	// We will be implementing diffuse lighting (Refer to "Lighting" in Notes.md for reasoning behind dot product!!!):
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - currPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);  // ReLU because we don't want negative color value

	// Specular lighting to simulate reflective properties of the surfaces:
	float specularLight = 0.50f;  // Arbitrary value
	
	// To store how much specular light we have at a certain angle.
	// Specular light intensity is inversely proportional to the angle between viewDirection and reflectionDirection:
	vec3 viewDirection = normalize(camPos - currPos);

	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/reflect.xhtml
	vec3 reflectionDirection = reflect(-lightDirection, normal);  // negative lightDirection because we want reflectionDirection to be towards the plane, not away from it.

	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);  // Definition and point-likeness of specular light is directly proportional to the power
	float specular = specularLight * specAmount;

	// Angle between current part that is lit and central light ray:
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);

	// Intensity at a current spot restricted to values between 0 and 1 to avoid weird values:
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);
	
	// Light intensity is applied only to diffuse and specular component to manipulate base colors and reflectivity:
	return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + texture(specular0, texCoord).r * specular * intensity) * lightColor;  // Specular map has only the red channel here. Therefore ".r" used.
}

void main() {
	// FragColor = pointLight();
	// FragColor = directionalLight();
	FragColor = spotLight();
}