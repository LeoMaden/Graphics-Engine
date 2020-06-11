#version 450 core
layout (location = 0) in vec3 aPosition;

uniform mat4 u_Transform;

void main() 
{
	gl_Position = u_Transform * vec4(aPosition, 1.0);
}