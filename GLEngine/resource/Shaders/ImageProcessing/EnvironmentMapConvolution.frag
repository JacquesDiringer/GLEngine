#version 330 core

const float PI = 3.14159265359f;
const float INVPI = 0.31830988618f;

in vec2 texCoordinates;
in vec3 cameraWorldRay;

out vec4 color;

// Environment map
uniform sampler2D envmap;

// Vector used to fetch in the envmap.
uniform vec3 currentFetchVector;

// Divisor, depending on the number of samples as well as the solid angle depending on the integration angle.
uniform float divisor;

uniform float roughnessInput;



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
	color = vec4(0, 0, 0, 1);

	// Compute the direction vector of the current pixel.
	// Here warpedTexCoords.x is phi, and warpedTexCoords.y is theta.
	vec2 warpedTexCoords = vec2((texCoordinates.x - 0.25f) * 2.0f * PI, texCoordinates.y * PI);
	float sinTheta = sin(warpedTexCoords.y);
	vec3 currentPixelDirection;
	currentPixelDirection.x = cos(warpedTexCoords.x) * sinTheta;
	currentPixelDirection.y = cos(warpedTexCoords.y);
	currentPixelDirection.z = sin(warpedTexCoords.x) * sinTheta;

	// Envmap fetch.
	float phiNormalized = 0.5f - atan(currentFetchVector.x, currentFetchVector.z) * INVPI * 0.5f;
	float thetaNormalized = acos(currentFetchVector.y) * INVPI;

	vec3 envmapSample = textureLod(envmap, vec2(phiNormalized, thetaNormalized), 0).xyz;

	vec3 contribution = CookTorrance(vec3(0), vec3(1), roughnessInput, currentPixelDirection, currentFetchVector, currentPixelDirection);
	color.xyz += envmapSample * contribution * divisor;
}