#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvCoordinates;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 instanceWorld;
//layout (location = 4) also instanceWorld
//layout (location = 5) also instanceWorld

out vec3 vertexColor;
out vec2 texCoordinates;
out vec3 worldNormal;
out float viewDepth;

uniform mat4 view, projection;

void main()
{
	vec4 viewPosition = view * instanceWorld * vec4(position, 1.0);

	vertexColor = vec3(1, 1, 1);
	texCoordinates = uvCoordinates;
	worldNormal = (instanceWorld * vec4(normal, 0)).xyz;
	viewDepth = viewPosition.z;
    gl_Position = projection * viewPosition;
}