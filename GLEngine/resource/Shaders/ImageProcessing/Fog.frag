#version 330 core

#define FOG_STEP_COUNT 10

in vec2 texCoordinates;

out vec4 color;

uniform float atmosphericMaxDistance = 20000.0f;
uniform vec3 atmosphericColor = vec3(0.0f, 0.45f, 1.0f);
uniform vec3 smogColor = vec3(0.3f, 0.3f, 0.3f);

uniform sampler2D inputTex;
// This is the geometry texture of the G Buffer, therefore the depth has to be fetched in the alpha channel.
uniform sampler2D depthTex;

// Matrices to rebuild the world coordinates thanks to the depth.
uniform mat4 iView, iProjection;
uniform vec3 cameraPosition;


float smogDensity(vec3 samplingPosition)
{
	return max(0, (-samplingPosition.y - 400.0f)  * 0.005f);
}

void main()
{
	vec3 texColor = texture2D(inputTex, texCoordinates).rgb;
	float depth = texture2D(depthTex, texCoordinates).a;

	vec4 screenCoordinates = vec4(texCoordinates.r * 2.0f - 1.0f, texCoordinates.g * 2.0f - 1.0f, 1, 1);
	vec4 viewCoordinates = iProjection * screenCoordinates;
	vec4 worldCoordinates = iView * viewCoordinates;
	vec3 worldFromDepth = normalize(worldCoordinates.xyz) * depth;
	worldFromDepth += cameraPosition;

	float smogFogSum = 0.0f;

	// Sample fog density functions step by step, from the camera to the furthest visible point (ie the point in the G-Buffer).
	vec3 stepVector = normalize(worldFromDepth - cameraPosition) * depth / FOG_STEP_COUNT;
	vec3 samplingPosition = cameraPosition;
	for(int stepId = 0; stepId < FOG_STEP_COUNT; stepId++)
	{
		// Accumulate fog.
		smogFogSum += smogDensity(samplingPosition);

		// Move to the next step coordinates.
		samplingPosition += stepVector;
	}

	smogFogSum /= FOG_STEP_COUNT;

	// Mix with the atmospheric haze.
	texColor = mix(texColor, atmosphericColor, clamp(depth / atmosphericMaxDistance, 0, 1));

	// Mix with the smog color.
	texColor = mix(texColor, smogColor, clamp(smogFogSum, 0, 1));

	color = vec4(texColor, 1);
}