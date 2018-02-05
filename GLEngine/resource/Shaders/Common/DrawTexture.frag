#version 330 core

in vec2 texCoordinates;

out vec4 color;

// Input texture to draw.
uniform sampler2D inputTexture;

void main()
{
	vec2 newTexCoords = vec2(texCoordinates.x, texCoordinates.y);
	color = texture(inputTexture, newTexCoords);
}