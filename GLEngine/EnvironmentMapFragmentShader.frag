#version 330 core

const float PI = 3.14159265359f;
const float INVPI = 0.31830988618f;

in vec3 vertexColor;
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

	float theta = -atan(worldNormal.x, worldNormal.z) * INVPI * 0.5f;
	float phi = - asin(worldNormal.y) * INVPI + 0.5f;

    vec3 finalColor = textureGrad(envmap, vec2(theta, phi), dFdx(abs(vec2(theta, phi))), dFdy(abs(vec2(theta, phi)))).rgb;

	geometryGTexture = vec4(0, 0, 0, 1);
	diffuseGTexture = vec3(0, 0, 0);
	specularRoughnessGTexture = vec4(0, 0, 0, 0);
    emissiveGTexture = finalColor;
}
