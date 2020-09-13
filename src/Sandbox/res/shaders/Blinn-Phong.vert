#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;

out vec3 v_FragPos;
out vec3 v_InterpNormal;

uniform mat4 u_ViewProjMatrix;

void main()
{
	v_FragPos = a_Position;
	v_InterpNormal = a_Normal;

	gl_Position = u_ViewProjMatrix * vec4(a_Position, 1.0);
}