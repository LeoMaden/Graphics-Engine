#version 450 core
out vec4 o_Color;

in vec4 v_Color;
in vec3 v_Normal;

void main() 
{
	o_Color = v_Color;
	//o_Color = vec4(v_Normal / 2.0 + 0.5, 1.0);
}