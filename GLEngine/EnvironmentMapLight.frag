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

// Environment map
uniform sampler2D envmap;

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
	float roughness = 0.000001f * specularRoughness.a;

	// Envmap fetch.
	// A linear interpolation between the reflexion direction (for 0 roughness materials) and the normal direction (for 1 roughness materials).
	vec3 envmapFetchDirection = mix(reflect(normalize(cameraWorldRay), normal), normal, roughness + 1);
	float theta = -atan(envmapFetchDirection.x, envmapFetchDirection.z) * INVPI * 0.5f;
	float phi = - asin(envmapFetchDirection.y) * INVPI + 0.5f;

    //vec3 envmapSample = textureGrad(envmap, vec2(theta, phi), dFdx(abs(vec2(theta, phi)) * (1 + roughness)), dFdy(abs(vec2(theta, phi)) * (1 + roughness))).rgb;
    vec3 envmapSample = textureGrad(envmap, vec2(theta, phi), dFdx(abs(vec2(theta, phi))), dFdy(abs(vec2(theta, phi)))).rgb;
    //vec3 envmapSample = texture(envmap, vec2(theta, phi)).rgb;

	vec3 finalColor = diffuseTexel * envmapSample;

    color = vec4(finalColor, 1.0f);
}
