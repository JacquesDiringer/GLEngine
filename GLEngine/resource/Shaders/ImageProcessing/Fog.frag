#version 330 core

in vec2 texCoordinates;

out vec4 color;

uniform float fogSMaxDistance = 2000.0f;
uniform vec3 fogCoolor = vec3(0.3f, 0.3f, 0.3f);

uniform sampler2D inputTex;
// This is the geometry texture of the G Buffer, therefore the depth has to be fetched in the alpha channel.
uniform sampler2D depthTex;

void main()
{
	vec3 texColor = texture2D(inputTex, texCoordinates).rgb;
	float depth = texture2D(depthTex, texCoordinates).a;
	//texColor = texColor * 0.000001f;

	texColor = mix(texColor, fogCoolor, clamp(depth / fogSMaxDistance, 0, 1));

	color = vec4(texColor, 1);
}