#version 330 core

in vec3 worldPosition;

out vec4 color;

// G-Buffer.
uniform sampler2D geometryGTexture;
uniform sampler2D diffuseGTexture;
uniform sampler2D specularRoughnessGTexture;

// The inverse of screen resolution.
uniform vec2 pixelSize;

uniform mat4 world, view, projection;

uniform vec3 cameraPosition;

// Light's power.
uniform vec3 power = vec3(1);

// CookTorrance lighting function.
// ref: http://ruh.li/GraphicsCookTorrance.html
vec3 CookTorrance(vec3 diffuseTexel, vec3 specularTexel, float roughnessTexel, vec3 normal, vec3 lightDirection, vec3 eyeDirection)
{
       // set important material values
       float roughnessValue = max(roughnessTexel, 0.01f); // 0 : smooth, 1: rough
 
	   float diffuseLuminance = 0.299f*diffuseTexel.r + 0.587f*diffuseTexel.g + 0.114f*diffuseTexel.b;
	   float specularLuminance = 0.299f*specularTexel.r + 0.587f*specularTexel.g + 0.114f*specularTexel.b;
	   float k = 0;
	   if (diffuseLuminance > 0)
	   {
			k = diffuseLuminance / (diffuseLuminance + specularLuminance); // fraction of diffuse reflection (specular reflection = 1 - k)
	   }
 
		// do the lighting calculation for each fragment.
       float NdotL = max(dot(normal, lightDirection), 0.0);
 
       float fresnel;
 
       float specular = 0.0;
       if (NdotL > 0.0)
       {
             vec3 eyeDir = normalize(eyeDirection);
 
             // calculate intermediary values
             vec3 halfVector = normalize(lightDirection + eyeDir);
             float NdotH = max(dot(normal, halfVector), 0.0);
             float NdotV = max(dot(normal, eyeDir), 0.001f); // note: this could also be NdotL, which is the same value
             float VdotH = max(dot(eyeDir, halfVector), 0.0);
             float mSquared = roughnessValue * roughnessValue;
 
             // geometric attenuation
             float NH2 = 2.0 * NdotH;
             float g1 = (NH2 * NdotV) / VdotH;
             float g2 = (NH2 * NdotL) / VdotH;
             float geoAtt = min(1.0, min(g1, g2));
 
             // roughness (or: microfacet distribution function)
             // beckmann distribution function
              float r1 = 1.0 / (4.0 * mSquared * pow(NdotH, 4.0));
             float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
             float roughness = r1 * exp(r2);
 
             //fresnel = textureLod(FresnelTexture, vec2(1 - k, 1 - NdotV), 0).r;
             fresnel = 1.0f;
 
             specular = (fresnel * geoAtt * roughness) / (NdotV * NdotL * 4.0f); // According to Cook-Torrance and Torrance-Sparrow the PI term here should rather be 4. Which we updated.
       }
 
       diffuseTexel.xyz *= k;
       specularTexel.xyz *= specular * (1.0f - k);
 
       return NdotL * (diffuseTexel + specularTexel);
}


void main()
{
	vec2 texCoords = vec2(gl_FragCoord.x * pixelSize.x, gl_FragCoord.y * pixelSize.y);

	color = vec4(0, 0, 0, 0);

	// Fetch in the geometry texture.
	vec4 geometryFetch = texture(geometryGTexture, texCoords);
	vec3 normal = geometryFetch.xyz;
	// Retrieve the depth, and stay aware that it is in view space.
	float depth = geometryFetch.w;

	// Use the depth to compute the world position of the current fragment.
	vec3 cameraRay = normalize(worldPosition - cameraPosition);
	vec3 gWorldPosition = cameraPosition + depth * cameraRay;

	// Fetch in the diffuse texture.
	vec3 diffuseFetch = texture(diffuseGTexture, texCoords).xyz;

	// Fetch in the specular roughness texture.
	vec4 specularRoughnessFetch = texture(specularRoughnessGTexture, texCoords);
	vec3 specularTexel = specularRoughnessFetch.xyz;
	float roughnessTexel = specularRoughnessFetch.w;

	vec3 lightPosition = world[3].xyz;
	vec3 lightDirection = lightPosition - gWorldPosition;
	float lightDistance = length(lightDirection);
	lightDirection = normalize(lightDirection);

	vec3 eyeDirection = normalize(cameraPosition - gWorldPosition);

	// Cook-Torrance contribution * light's power.
	color.xyz += power * CookTorrance(diffuseFetch, specularTexel, roughnessTexel, normal, lightDirection, eyeDirection);

	// Divide by the square of the distance from the light to the surface.
	// For distance attenuation.
	color.xyz /= pow(lightDistance, 2);
}