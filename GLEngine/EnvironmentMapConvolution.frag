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
	// Here warpedTexCoords.x is theta, and warpedTexCoords.y is phi.
	vec2 warpedTexCoords = vec2((texCoordinates.x + 0.25f) * 2.0f * PI, texCoordinates.y * PI);
	float sinTheta = sin(warpedTexCoords.y);
	vec3 currentPixelDirection;
	currentPixelDirection.x = cos(warpedTexCoords.x) * sinTheta;
	currentPixelDirection.y = cos(warpedTexCoords.y);
	currentPixelDirection.z = sin(warpedTexCoords.x) * sinTheta;

	// Envmap fetch.
	float theta = 0.5f - atan(currentFetchVector.x, currentFetchVector.z) * INVPI * 0.5f;
	float phi = 0.5f + asin(currentFetchVector.y) * INVPI;

	vec3 envmapSample = textureLod(envmap, vec2(theta, phi), 0).xyz;

	// Compute final fetch contribution, according to the cos theta of the two vector, as well as the 
	float cosVectorsAngle = dot(currentPixelDirection, currentFetchVector);

	// The vector angle is set at maximum to the integrationAngle, to avoid circle loops when rescaling to adapt the cos to the integration angle.
	float vectorsAngle = min(acos(cosVectorsAngle), integrationAngle);

	// Rescale of the vectorsAngle, to fit in the integrationAngle while still having a nice cosinus behaviour.
	float contribution = max(0, cos(vectorsAngle * PI * 0.5f / integrationAngle));

	color.xyz += envmapSample * contribution * divisor;
}