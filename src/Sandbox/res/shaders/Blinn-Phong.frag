#version 450 core

out vec4 o_Color;

in vec3 v_FragPos;
in vec3 v_InterpNormal;


struct Material
{
	vec3 AmbientColor;
	vec3 DiffuseColor;
	vec3 SpecularColor;

	float Shininess; // Specular exponent.
};

struct PointLight
{        
	vec3 Position;

    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;

    float Constant;
    float Linear;
    float Quadratic;
};

uniform Material u_Material;
uniform PointLight u_PointLight;
uniform vec3 u_ViewPos;

void main()
{
	// Ambient lighting.
	vec3 ambientColor = u_Material.AmbientColor * u_PointLight.Ambient;

	// Diffuse lighting.
	vec3 lightDir = normalize(v_FragPos - u_PointLight.Position); // Light -> frag

	float diffuseFactor = max(dot(v_InterpNormal, -lightDir), 0.0);
	vec3 diffuseColor = diffuseFactor * u_Material.DiffuseColor * u_PointLight.Diffuse;

	// Specular lighting.
	vec3 viewDir = normalize(v_FragPos - u_ViewPos); // View pos -> frag
	vec3 reflectDir = reflect(lightDir, v_InterpNormal);

	float specularFactor = pow(max(dot(reflectDir, -viewDir), 0.0), u_Material.Shininess);
	vec3 specularColor = specularFactor * u_Material.SpecularColor * u_PointLight.Specular;


	vec3 col = ambientColor + diffuseColor + specularColor;

	// Attenuate lighting.
	float d = length(u_PointLight.Position - v_FragPos);
	col /= (u_PointLight.Constant + (d * u_PointLight.Linear) + (d * d * u_PointLight.Quadratic));

	o_Color = vec4(col, 1.0);
}