#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in vec3 a_Normal;

out vec3 v_FragPos;
out vec3 v_InterpNormal;
out vec2 v_TexCoord;

uniform mat4 u_ViewProjMatrix;
uniform mat4 u_Transform;

void main()
{
	v_FragPos = vec3(u_Transform * vec4(a_Position, 1.0));
	v_InterpNormal = a_Normal;
	v_TexCoord = a_TexCoord;

	gl_Position = u_ViewProjMatrix * u_Transform * vec4(a_Position, 1.0);
}