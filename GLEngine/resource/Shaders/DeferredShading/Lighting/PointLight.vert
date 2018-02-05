#version 330 core
  
layout (location = 0) in vec3 position;

out vec3 worldPosition;

uniform mat4 world, view, projection;

void main()
{
	vec4 v4worldPosition = world * vec4(position, 1.0);
	worldPosition = v4worldPosition.xyz;

    gl_Position = projection * view * v4worldPosition;
}