#version 330 core

in vec2 texCoordinates;
in vec3 worldNormal;
in vec3 worldPosition;

layout(location = 0) out vec4 geometryGTexture;
layout(location = 1) out vec3 diffuseGTexture;
layout(location = 2) out vec4 specularRoughnessGTexture;
layout(location = 3) out vec3 emissiveGTexture;

uniform float time = 0;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D roughnessTexture;

uniform mat4 view;

void main()
{
	vec2 newCoords = vec2(texCoordinates.x, 1 - texCoordinates.y);

	float viewDepth = length((view * vec4(worldPosition, 1)).xyz);
	geometryGTexture = vec4(worldNormal, viewDepth);
	diffuseGTexture = texture(diffuseTexture, newCoords).rgb;
	specularRoughnessGTexture.rgb = texture(specularTexture, newCoords).rgb;
	specularRoughnessGTexture.a = texture(roughnessTexture, newCoords).r;
    emissiveGTexture = vec3(0, 0, 0);

	// Remove gamma correction.
	diffuseGTexture = pow(diffuseGTexture, vec3(2.2f));
} 