#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvCoordinates;
layout (location = 2) in vec3 normal;

out vec3 vertexColor;
out vec2 texCoordinates;
out vec3 worldNormal;

uniform mat4 world, view, projection;

void main()
{
	vertexColor = vec3(1, 1, 1);
	texCoordinates = uvCoordinates;
	worldNormal = (world * vec4(normal, 0)).xyz;
    gl_Position = projection * view * world * vec4(position, 1.0);
}