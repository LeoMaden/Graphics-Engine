#version 450 core
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in float a_TexSlot;
//layout (location = 2) in float a_TilingFactor;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexSlot;

uniform mat4 u_Transform;

void main()
{
	v_TexCoord = a_TexCoord;
	v_TexSlot = a_TexSlot;
	gl_Position = u_Transform * vec4(a_Position, 1.0);
}