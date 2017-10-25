#version 330 core

in vec2 texCoordinates;

out vec4 color;

uniform sampler2D inputTex;
uniform sampler2D lensChromaticAberrationTex;
uniform int uGhosts = 5; // number of ghost samples
uniform float uGhostDispersal = 1.0f; // dispersion factor
uniform float uBias = 2;


void main()
{
	vec2 texcoord = -texCoordinates + vec2(1.0);
 
   // ghost vector to image centre:
      vec2 ghostVec = (vec2(0.5) - texcoord) * uGhostDispersal;
   
	// sample ghosts:  
    for (int i = 0; i < uGhosts; ++i)
	{ 
        vec2 offset = fract(texcoord + ghostVec * float(i));

		// Falloff from the center of the image.
		// This way, only spots from the center will generate ghosts.
		float weight = length(vec2(0.5) - offset) / length(vec2(0.5));
		weight = pow(1.0 - weight, 10.0);
  
        color.xyz += max(vec3(0.0), textureLod(inputTex, offset, 3.0f).xyz - vec3(uBias)) * weight;
    }

	// We multiply the ghosts by a chromatic aberration color, fetched in a texture.
	vec3 chromaticAberration = texture(lensChromaticAberrationTex, vec2(length(vec2(0.5) - texcoord) / length(vec2(0.5)))).xyz;
	color.xyz *= chromaticAberration;
}