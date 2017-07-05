#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvCoordinates;
layout (location = 2) in vec3 normal;

out vec3 vertexColor;
out vec2 texCoordinates;
out vec3 worldNormal;
out float viewDepth;

uniform mat4 world, view, projection;

void main()
{
	vec4 viewPosition = view * world * vec4(position, 1.0);

	vertexColor = vec3(1, 1, 1);
	texCoordinates = uvCoordinates;
	worldNormal = (world * vec4(normal, 0)).xyz;
	viewDepth = -viewPosition.z;
    gl_Position = projection * viewPosition;
}