#version 450 core
out vec4 o_Color;

in vec2 v_TexCoord;
in float v_TexSlot;

uniform sampler2D u_Textures[32];

void main() 
{
	//o_Color = vec4(v_TexSlot, 1.0, 0.0, 1.0);
	o_Color = texture(u_Textures[int(v_TexSlot)], v_TexCoord);

}