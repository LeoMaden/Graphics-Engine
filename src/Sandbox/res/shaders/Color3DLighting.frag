#version 450 core
out vec4 o_Color;

in vec4 v_Color;
in vec3 v_Normal;
in vec3 v_Position;

uniform vec3 u_ViewPos;

struct Material
{
	vec3 Color;
	vec3 Specular;

	float Shininess;
};

struct DirectionalLight
{
	vec3 Direction; // Must be normalised

	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
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
uniform DirectionalLight u_DirLight;
uniform PointLight u_PointLight;

vec4 CalcLighting(DirectionalLight light)
{
	vec3 reflectDir = reflect(light.Direction, v_Normal);
	vec3 viewDir = normalize(u_ViewPos - v_Position);

	vec3 ambient = light.Ambient * vec3(v_Color);
	//vec3 ambient = light.Ambient * u_Material.Color;

	float diff = max(dot(v_Normal, -light.Direction), 0.0);
	vec3 diffuse = light.Diffuse * (diff * vec3(v_Color));
	//vec3 diffuse = light.Diffuse * diff * u_Material.Color;

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
	vec3 specular = light.Specular * (spec * vec3(v_Color));
	//vec3 specular = light.Specular * spec * u_Material.Specular;

	vec3 total = ambient + diffuse + specular;
	return vec4(total, 1.0);
}

vec4 CalcLighting(PointLight light)
{
	float dist = length(light.Position - v_Position);
	float attenuation = 1.0 / (light.Constant + light.Linear * dist + light.Quadratic * dist * dist);

	vec3 lightDir = normalize(light.Position - v_Position);
	vec3 viewDir = normalize(u_ViewPos - v_Position);
	vec3 reflectDir = reflect(-lightDir, v_Normal);
	
	vec3 ambient = light.Ambient * vec3(v_Color);
	//vec3 ambient = light.Ambient * u_Material.Color;

	float diff = max(dot(v_Normal, lightDir), 0.0);
	vec3 diffuse = light.Diffuse * (diff * vec3(v_Color));
	//vec3 diffuse = light.Diffuse * diff * u_Material.Color;

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.Shininess);
	vec3 specular = light.Specular * (spec * vec3(v_Color));
	//vec3 specular = light.Specular * spec * u_Material.Specular;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 total = ambient + diffuse + specular;
	return vec4(total, 1.0);
}

void main() 
{
	//o_Color = v_Color;
	o_Color += CalcLighting(u_DirLight);
	o_Color += CalcLighting(u_PointLight);
	//o_Color = vec4(fract(v_Normal), 1.0);
}