#version 330 core

const float PI = 3.14159265359f;
const float INVPI = 0.31830988618f;

in vec2 texCoordinates;
in vec3 cameraWorldRay;

out vec4 color;

// Environment map
uniform sampler2D envmap;

// Vector used to fetch in the envmap.
uniform vec3 currentFetchVector;

// Integration angle, 1 means the lambertian half sphere, 0 should never be used and stands for a Dirac.
uniform float integrationAngle;

// Divisor, depending on the number of samples as well as the solid angle depending on the integration angle.
uniform float divisor;

void main()
{
	color = vec4(0, 0, 0, 1);

	// Compute the direction vector of the current pixel.
	// Here warpedTexCoords.x is phi, and warpedTexCoords.y is theta.
	vec2 warpedTexCoords = vec2((texCoordinates.x + 0.25f) * 2.0f * PI, texCoordinates.y * PI);
	float sinTheta = sin(warpedTexCoords.y);
	vec3 currentPixelDirection;
	currentPixelDirection.x = -cos(warpedTexCoords.x) * sinTheta;
	currentPixelDirection.y = cos(warpedTexCoords.y);
	currentPixelDirection.z = sin(warpedTexCoords.x) * sinTheta;

	// Envmap fetch.
	float theta = atan(currentFetchVector.x, currentFetchVector.z) * INVPI * 0.5f;
	float phi = - asin(currentFetchVector.y) * INVPI + 0.5f;

	vec3 envmapSample = textureLod(envmap, vec2(theta, phi), 0).xyz;

	float contribution = max(0, dot(currentPixelDirection, currentFetchVector) -1 + integrationAngle);
	color.xyz += envmapSample * contribution * divisor;
}