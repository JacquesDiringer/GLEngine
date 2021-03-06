#version 330 core

#define FLT_MAX 3.402823466e+38

const float PI = 3.14159265359f;
const float INVPI = 0.31830988618f;

in vec2 texCoordinates;
in vec3 worldNormal;

layout(location = 0) out vec4 geometryGTexture;
layout(location = 1) out vec3 diffuseGTexture;
layout(location = 2) out vec4 specularRoughnessGTexture;
layout(location = 3) out vec3 emissiveGTexture;

uniform sampler2D envmap;

void main()
{
	vec3 worldNormal = normalize(worldNormal);

	float phiNormalized = 0.5f - atan(worldNormal.x, worldNormal.z) * INVPI * 0.5f;
	float thetaNormalized = acos(worldNormal.y) * INVPI;

	vec2 fetchCoordinates = vec2(phiNormalized, thetaNormalized);

    vec3 finalColor = textureGrad(envmap, fetchCoordinates, dFdx(abs(fetchCoordinates)), dFdy(abs(fetchCoordinates))).rgb;

	// Remove gamma correction.
	finalColor = pow(finalColor, vec3(2.2f));

	// The distance will be the max value of a 16 bits float, since the geometry texture has a RGBAF16 pixel format.
	geometryGTexture = vec4(0, 0, 0, FLT_MAX);
	diffuseGTexture = vec3(0, 0, 0);
	specularRoughnessGTexture = vec4(0, 0, 0, 0);
    emissiveGTexture = finalColor;
}
