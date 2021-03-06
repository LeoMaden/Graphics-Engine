#version 450 core

// Number of each type of light.
#define NUM_POINT_LIGHTS 1
#define NUM_DIR_LIGHTS 0

// 0 = Color only, 1 = Color * Texture, 2 = Texture only.
#define MATERIAL_DIFFUSE 2
#define MATERIAL_SPECULAR 0
#define MATERIAL_SHININESS 0


// OUT
out vec4 o_Color;


// IN
in vec3 v_FragPos;
in vec3 v_InterpNormal;
in vec2 v_TexCoord;


// STRUCTS
struct Material
{

#if MATERIAL_DIFFUSE <= 1
	vec3 AmbientColor;
	vec3 DiffuseColor;
#endif
#if MATERIAL_DIFFUSE >= 1
	sampler2D DiffuseMap;
#endif

#if MATERIAL_SPECULAR <= 1
	vec3 SpecularColor;
#endif
#if MATERIAL_SPECULAR >= 1
	sampler2D SpecularMap;
#endif

#if MATERIAL_SHININESS <= 1
	float Shininess; // Specular exponent.
#endif
#if MATERIAL_SHININESS >= 1
	sampler2D ShininessMap;
#endif

};

#if NUM_POINT_LIGHTS > 0
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
#endif


// UNIFORMS
uniform Material u_Material;
uniform vec3 u_ViewPos;

#if NUM_POINT_LIGHTS > 0
uniform PointLight u_PointLights[NUM_POINT_LIGHTS];
#endif


// FUNCTIONS
vec3 CalcLighting(PointLight light)
{

#if MATERIAL_DIFFUSE == 0
	vec3 matAmbientColor = u_Material.AmbientColor;
	vec3 matDiffuseColor = u_Material.DiffuseColor;
#elif MATERIAL_DIFFUSE == 1
	vec3 matAmbientColor = u_Material.AmbientColor * vec3(texture2D(u_Material.DiffuseMap, v_TexCoord));
	vec3 matDiffuseColor = u_Material.DiffuseColor * vec3(texture2D(u_Material.DiffuseMap, v_TexCoord));
#elif MATERIAL_DIFFUSE == 2
	vec3 matAmbientColor = vec3(texture2D(u_Material.DiffuseMap, v_TexCoord));
	vec3 matDiffuseColor = vec3(texture2D(u_Material.DiffuseMap, v_TexCoord));
#endif

#if MATERIAL_SPECULAR == 0
	vec3 matSpecularColor = u_Material.SpecularColor;
#elif MATERIAL_SPECULAR == 1
	vec3 matSpecularColor = u_Material.SpecularColor * vec3(texture2D(u_Material.SpecularMap, v_TexCoord));
#elif MATERIAL_SPECULAR == 2
	vec3 matSpecularColor = vec3(texture2D(u_Material.SpecularMap, v_TexCoord));
#endif

//#if MATERIAL_SHININESS == 0
//	float shininess = u_Material.Shininess;
//#elif MATERIAL_SHININESS == 1
//	float shininess = u_Material.Shininess * texture2D(u_Material.ShininessMap, v_TexCoord);
//#elif MATERIAL_SHININESS == 2
//	float shininess = texture2D(u_Material.ShininessMap, v_TexCoord);
//#endif


	vec3 lightDir = normalize(light.Position - v_FragPos);
	vec3 viewDir = normalize(u_ViewPos - v_FragPos);
	vec3 halfDir = normalize(lightDir + viewDir);

	vec3 normal = normalize(v_InterpNormal);
	vec3 reflectDir = reflect(-lightDir, normal);

	// Ambient lighting.
	vec3 ambient = light.Ambient * matAmbientColor;

	// Diffuse lighting.
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.Diffuse * diff * matDiffuseColor;

	// Specular lighting.
	float spec = pow(max(dot(normal, halfDir), 0.0), u_Material.Shininess);
	vec3 specular = light.Specular * spec * matSpecularColor;

	vec3 col = ambient + diffuse + specular;

	// Attenuate lighting.
	float d = length(light.Position - v_FragPos);
	col /= (light.Constant + (d * light.Linear) + (d * d * light.Quadratic));

	return col;
}


void main()
{
	vec3 color = vec3(0.0);

	for (int i = 0; i < NUM_POINT_LIGHTS; i++)
	{
		PointLight pl = u_PointLights[i];
		color += CalcLighting(pl);
	}

	o_Color = vec4(color, 1.0);
}