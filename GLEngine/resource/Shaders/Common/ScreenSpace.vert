#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvCoordinates;

out vec2 texCoordinates;
out vec3 cameraWorldRay;

// Inverse of the view and projection matrices.
uniform mat4 iView, iProjection;

void main()
{
	texCoordinates = uvCoordinates;
    gl_Position = vec4(position, 1.0);
	cameraWorldRay = (iView * iProjection * vec4(position.x, position.y, 1.0, 1.0)).xyz;
}