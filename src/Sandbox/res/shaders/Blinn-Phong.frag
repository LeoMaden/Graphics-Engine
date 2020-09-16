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
	vec3 lightDir = normalize(u_PointLight.Position - v_FragPos);
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 halfDir = normalize(lightDir + viewDir);

	vec3 normal = normalize(v_InterpNormal);
	vec3 reflectDir = reflect(-lightDir, normal);

	// Ambient lighting.
	vec3 ambient = u_PointLight.Ambient * u_Material.AmbientColor;

	// Diffuse lighting.
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = u_PointLight.Diffuse * diff * u_Material.DiffuseColor;

	// Specular lighting.
	float spec = pow(max(dot(normal, halfDir), 0.0), u_Material.Shininess);
	vec3 specular = u_PointLight.Specular * spec * u_Material.SpecularColor;

	vec3 col = ambient + diffuse + specular;

	// Attenuate lighting.
	float d = length(u_PointLight.Position - v_FragPos);
	col /= (u_PointLight.Constant + (d * u_PointLight.Linear) + (d * d * u_PointLight.Quadratic));

	o_Color = vec4(col, 1.0);
}