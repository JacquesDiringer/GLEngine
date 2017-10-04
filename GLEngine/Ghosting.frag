#version 330 core

in vec2 texCoordinates;

out vec4 color;

uniform sampler2D inputTex;
uniform int uGhosts = 5; // number of ghost samples
uniform float uGhostDispersal = 1.0f; // dispersion factor


void main()
{
	vec2 texcoord = -texCoordinates + vec2(1.0);
 
   // ghost vector to image centre:
      vec2 ghostVec = (vec2(0.5) - texcoord) * uGhostDispersal;
   
	// sample ghosts:  
    for (int i = 0; i < uGhosts; ++i)
	{ 
        vec2 offset = fract(texcoord + ghostVec * float(i));
  
        color.xyz += max(vec3(0.0), texture(inputTex, offset).xyz - vec3(1));
    }
}