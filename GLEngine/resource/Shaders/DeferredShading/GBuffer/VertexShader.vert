#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvCoordinates;
layout (location = 2) in vec3 normal;

out vec2 texCoordinates;
out vec3 worldNormal;
out vec3 worldPosition;

uniform mat4 world, view, projection;

void main()
{
	vec4 v4worldPosition = world * vec4(position, 1.0);
	worldPosition = v4worldPosition.xyz;

	texCoordinates = uvCoordinates;
	worldNormal = (world * vec4(normal, 0)).xyz;
    gl_Position = projection * view * v4worldPosition;
}