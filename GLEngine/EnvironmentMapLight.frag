#version 330 core

const float PI = 3.14159265359f;
const float INVPI = 0.31830988618f;

in vec2 texCoordinates;
in vec3 cameraWorldRay;

out vec4 color;

// G-Buffer
uniform sampler2D geometryGTexture;
uniform sampler2D diffuseGTexture;
uniform sampler2D specularRoughnessGTexture;
uniform sampler2D emissiveGTexture;

// Image based lighting map.
uniform sampler3D iblMap;

void main()
{
	// Geometry fetch.
    vec4 geometryTexel = texture(geometryGTexture, texCoordinates);
	vec3 normal = normalize(geometryTexel.rgb);
	float viewDepth = geometryTexel.a;

	// Diffuse fetch.
    vec3 diffuseTexel = texture(diffuseGTexture, texCoordinates).rgb;

	// Specular and roughness fetch.
	vec4 specularRoughness = texture(specularRoughnessGTexture, texCoordinates);
	float roughness = specularRoughness.a;

	// Envmap fetch.
	// A linear interpolation between the reflexion direction (for 0 roughness materials) and the normal direction (for 1 roughness materials).
	vec3 envmapFetchDirection = normalize(mix(reflect(normalize(cameraWorldRay), normal), normal, roughness));

	float theta = 0.5f - atan(envmapFetchDirection.x, envmapFetchDirection.z) * INVPI * 0.5f;
	float phi = 0.5f + asin(envmapFetchDirection.y) * INVPI;

	vec3 fetchCoordinates = vec3(theta, phi, roughness);

    vec3 envmapSample = textureGrad(iblMap, fetchCoordinates, dFdx(abs(fetchCoordinates)), dFdy(abs(fetchCoordinates))).rgb;

	vec3 finalColor = diffuseTexel * envmapSample;

    color = vec4(finalColor, 1.0f);
}
