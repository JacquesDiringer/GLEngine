#version 330 core

in vec2 texCoordinates;

out vec4 color;

uniform sampler2D inputTex;

uniform float weight[6] = float[] (1, 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

float sizeFactor = 1.0f;

void main()
{
	vec3 texColor = texture2D(inputTex, texCoordinates).rgb;
	color = vec4(texColor, 1);
	//color = vec4(0,0,0, 1);

	vec2 tex_offset = sizeFactor / textureSize(inputTex, 0); // gets size of single texel

	for(int i = -5; i < 6; ++i)
	{
		for(int j = -5; j < 6; ++j)
		{
			if(i != 0 || j != 0)
			{
				vec3 fetchedColor = texture(inputTex, texCoordinates + vec2(tex_offset.x * i, tex_offset.y * j)).rgb;
				//color.rgb += fetchedColor * weight[abs(i)] * weight[abs(j)] * 0.5f;
				color.rgb += (fetchedColor * pow(abs(length(fetchedColor) - 0.8f), 1.0f))  / 100;
			}
		}
	}

	//if(length(texColor) > 1.4f)
	//{
	//	color += vec4(texColor.r, texColor.g, texColor.b, 1);
	//}
}