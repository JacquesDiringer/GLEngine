#version 330 core

in vec3 vertexColor;
in vec2 texCoordinates;
in vec3 worldNormal;
in float viewDepth;

layout(location = 0) out vec4 geometryGTexture;
layout(location = 1) out vec3 diffuseGTexture;
layout(location = 2) out vec4 specularRoughnessGTexture;
layout(location = 3) out vec3 emissiveGTexture;

uniform float time = 0;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D roughnessTexture;

void main()
{
	vec2 newCoords = vec2(texCoordinates.x, 1 - texCoordinates.y);

	geometryGTexture = vec4(worldNormal, viewDepth);
	diffuseGTexture = texture(diffuseTexture, newCoords).rgb + vec3(1);
	specularRoughnessGTexture.rgb = texture(specularTexture, newCoords).rgb;
	specularRoughnessGTexture.a = texture(roughnessTexture, newCoords).r;
    emissiveGTexture = vec3(0, 0, 0);
} 