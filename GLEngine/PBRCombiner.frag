#version 330 core

in vec2 texCoordinates;

out vec4 color;

// G-Buffer
uniform sampler2D specularRoughnessGTexture;
uniform sampler2D emissiveGTexture;

// Lighting map.
uniform sampler2D lightingTexture;

void main()
{
    vec3 lightingTexel = texture(lightingTexture, texCoordinates).rgb;
    vec3 emissiveTexel = texture(emissiveGTexture, texCoordinates).rgb;

	color = vec4(lightingTexel + emissiveTexel, 1.0f);
}