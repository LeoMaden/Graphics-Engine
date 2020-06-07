#version 450 core
out vec4 vColor;

uniform vec3 uColor;

void main() 
{
	vColor = vec4(uColor, 1.0);
}