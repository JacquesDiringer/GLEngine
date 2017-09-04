#version 330 core

in vec2 texCoordinates;

out vec4 color;

uniform sampler2D inputTex;

void main()
{
	vec3 texColor = texture2D(inputTex, texCoordinates).rgb;
	texColor = pow(texColor, vec3(0.45f));
	color = vec4(texColor, 1);
}