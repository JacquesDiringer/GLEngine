#version 330 core

in vec3 vertexColor;
in vec2 texCoordinates;
in vec3 worldNormal;

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
	geometryGTexture = vec4(worldNormal, 1.0f);
	diffuseGTexture = vec3(1, 1, 1);
	specularRoughnessGTexture = vec4(0, 0, 0, 0);
    emissiveGTexture = vec3(0, 0, 0);
} 