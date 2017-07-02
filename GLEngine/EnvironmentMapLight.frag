#version 330 core

const float PI = 3.14159265359f;
const float INVPI = 0.31830988618f;

in vec2 texCoordinates;

out vec4 color;

// G-Buffer
uniform sampler2D geometryGTexture;
uniform sampler2D diffuseGTexture;
uniform sampler2D specularRoughnessGTexture;
uniform sampler2D emissiveGTexture;

void main()
{
	// Geometry fetch.
    vec4 geometryTexel = texture(geometryGTexture, texCoordinates);
	vec3 normal = geometryTexel.rgb;
	float depth = geometryTexel.a;

	// Diffuse fetch.
    vec3 diffuseTexel = texture(diffuseGTexture, texCoordinates).rgb;

	float illuminance = dot(normal, normalize(vec3(1.0, 1.0, 1.0)));

	vec3 finalColor = diffuseTexel * illuminance;

    color = vec4(finalColor, 1.0f);
}
