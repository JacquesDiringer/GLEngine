#version 330 core

in vec2 texCoordinates;

out vec4 color;

uniform sampler2D inputTex;
uniform vec2 uPixelSize;
uniform vec2 uSamplingAxis;
uniform int uSamplesCount = 5;


void main()
{
	color = vec4(0,0,0,1);

	int halfSamplesCount = int(uSamplesCount*0.5f);

	for(int i = -halfSamplesCount; i < halfSamplesCount; ++i)
	{
		vec2 warpedTexCoords = texCoordinates + i * uPixelSize * uSamplingAxis;
		color.xyz += textureLod(inputTex, warpedTexCoords, 0.0f).xyz;	
	}

	color.xyz /= uSamplesCount;
}