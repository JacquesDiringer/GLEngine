#version 330 core

in vec2 texCoordinates;

out vec4 color;

// G-Buffer
uniform sampler2D emissiveGTexture;

// Lighting map.
uniform sampler2D lightingTexture;

void main()
{
	vec2 newTexCoords = vec2(texCoordinates.x, texCoordinates.y);
    vec3 lightingTexel = texture(lightingTexture, newTexCoords).rgb;
    vec3 emissiveTexel = texture(emissiveGTexture, newTexCoords).rgb;

	color = vec4(lightingTexel + emissiveTexel, 1.0f);

	// Add gamma correction.
	color = pow(color, vec4(0.45f));
}