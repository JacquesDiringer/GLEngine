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
    vec3 diffuse = texture(diffuseGTexture, texCoordinates).rgb;

	// Specular and roughness fetch.
	vec4 specularRoughness = texture(specularRoughnessGTexture, texCoordinates);
	vec3 specular = specularRoughness.rgb;
	float roughness = specularRoughness.a;
	float diffuseLuminance = 0.299f*diffuse.r + 0.587f*diffuse.g + 0.114f*diffuse.b;
	float specularLuminance = 0.299f*specular.r + 0.587f*specular.g + 0.114f*specular.b;
	float specularProportion = 0;
	if (specularLuminance > 0)
	{
		specularProportion = specularLuminance / (diffuseLuminance + specularLuminance);
	}

	// Envmap diffuse fetch.
	float phiNormalized = 0.5f - atan(normal.x, normal.z) * INVPI * 0.5f;
	float thetaNormalized = acos(normal.y) * INVPI;

	// Z should always be one here, to get the half sphere convolution layer.
	vec3 diffuseFetchCoordinates = vec3(phiNormalized, thetaNormalized, 1.0f);

    vec3 envmapDiffuseSample = textureGrad(iblMap, diffuseFetchCoordinates, dFdx(abs(diffuseFetchCoordinates)), dFdy(abs(diffuseFetchCoordinates))).rgb;


	// Envmap specular fetch.
	// A linear interpolation between the reflexion direction (for 0 roughness materials) and the normal direction (for 1 roughness materials).
	vec3 envmapSpecularFetchDirection = normalize(mix(reflect(normalize(cameraWorldRay), normal), normal, roughness));

	phiNormalized = 0.5f - atan(envmapSpecularFetchDirection.x, envmapSpecularFetchDirection.z) * INVPI * 0.5f;
	thetaNormalized = acos(envmapSpecularFetchDirection.y) * INVPI;

	vec3 specularFetchCoordinates = vec3(phiNormalized, thetaNormalized, roughness);

    vec3 envmapSpecularSample = textureGrad(iblMap, specularFetchCoordinates, dFdx(abs(specularFetchCoordinates)), dFdy(abs(specularFetchCoordinates))).rgb;

	vec3 finalColor = mix(diffuse * envmapDiffuseSample, specular * envmapSpecularSample, specularProportion);

    color = vec4(finalColor, 1.0f);
}
