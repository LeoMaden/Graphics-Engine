#version 450 core
out vec4 vColor;

in vec3 oColor;

void main() 
{
	vColor = vec4(oColor, 1.0);
}