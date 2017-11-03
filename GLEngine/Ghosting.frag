#version 330 core

in vec2 texCoordinates;

out vec4 color;

uniform sampler2D inputTex;
uniform sampler2D lensChromaticAberrationTex;
uniform float uMipLevelForGhosts;
uniform int uGhosts = 4; // number of ghost samples
uniform float uGhostDispersal = 1.0f; // dispersion factor
uniform float uBias = 2;
uniform float uHaloWidth = 0.2f;
uniform float uDistortion = 7.0f;
uniform vec2 uPixelSize;


vec3 textureDistorted(
      in sampler2D tex,
      in vec2 texcoord,
      in vec2 direction, // direction of distortion
      in vec3 distortion // per-channel distortion factor
   ) {
      return vec3(
         textureLod(tex, texcoord + direction * distortion.r, uMipLevelForGhosts).r,
         textureLod(tex, texcoord + direction * distortion.g, uMipLevelForGhosts).g,
         textureLod(tex, texcoord + direction * distortion.b, uMipLevelForGhosts).b
      );
   }

void main()
{
	vec2 texcoord = -texCoordinates + vec2(1.0);
 
   // ghost vector to image centre:
    vec2 ghostVec = (vec2(0.5) - texcoord) * uGhostDispersal;

	vec3 distortion = vec3(-uPixelSize.x * uDistortion, 0.0, uPixelSize.x * uDistortion);

	vec2 direction = normalize(ghostVec);
   
	// sample ghosts:  
    for (int i = 0; i < uGhosts; ++i)
	{ 
        vec2 offset = fract(texcoord + ghostVec * float(i));

		// Falloff from the center of the image.
		// This way, only spots from the center will generate ghosts.
		float weight = length(vec2(0.5) - offset) / length(vec2(0.5));
		weight = pow(1.0 - weight, 10.0);
  
        color.xyz += max(vec3(0.0), textureDistorted(inputTex, offset, direction, distortion ).xyz - vec3(uBias)) * weight;
    }

	// sample halo:
   vec2 haloVec = normalize(ghostVec) * uHaloWidth;
   float weight = length(vec2(0.5) - fract(texcoord + haloVec)) / length(vec2(0.5));
   weight = pow(1.0 - weight, 5.0);
   color.xyz += (max(vec3(0.0), textureDistorted(inputTex, texcoord + haloVec, direction, distortion).xyz - vec3(uBias))) * weight;

   // We multiply the ghosts by a chromatic aberration color, fetched in a texture.
	vec3 chromaticAberration = texture(lensChromaticAberrationTex, vec2(length(vec2(0.5) - texcoord) / length(vec2(0.5)))).xyz;
	color.xyz *= chromaticAberration;
}