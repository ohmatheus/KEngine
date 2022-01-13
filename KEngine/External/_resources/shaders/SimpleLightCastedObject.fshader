#version 410 core

#define NR_POINT_LIGHTS 4
#define NR_SPOT_LIGHTS 4

//----------------------------------------------------------
struct Material
{
    sampler2D 				diffuse;
    sampler2D				specular;
    float 					shininess;
};

//----------------------------------------------------------
struct DirectionalLight
{
    vec3 					direction;
    vec3 					ambient;
    vec3 					diffuse;
    vec3 					specular;
};

//----------------------------------------------------------
struct PointLight
{
    vec3 					position;
	
    vec3 					ambient;
    vec3 					diffuse;
    vec3 					specular;

	float 					constant;
    float 					linear;
    float 					quadratic;
};

//----------------------------------------------------------
struct SpotLight
{
	vec3 					position;
	vec3 					direction;
	
    vec3 					ambient;
    vec3 					diffuse;
    vec3 					specular;

	float 					cutoff;
	float 					outerCutoff;
	
	float 					constant;
    float 					linear;
    float 					quadratic;
};

//----------------------------------------------------------
//----------------------------------------------------------
//----------------------------------------------------------

in 		vec3 				Normal; 
in 		vec3 				FragPos;
in 		vec2 				TexCoords;
out 	vec4 				FragColor;

//----------------------------------------------------------
uniform vec3 				viewPos;
uniform Material 			material;
uniform SpotLight			light;

uniform DirectionalLight	dirLight;
uniform PointLight 			pointLights[NR_POINT_LIGHTS];
uniform SpotLight 			spotLights[NR_SPOT_LIGHTS];

//----------------------------------------------------------
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}

//----------------------------------------------------------
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	// ambient
	vec3 	ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 	lightDir = normalize(light.position - FragPos);
	
	float 	distance = length(light.position - FragPos);
	float 	attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	ambient *= attenuation;
	
	float 	theta = dot(lightDir, normalize(-light.direction));
    
	vec3 	result;
	
	if(theta > light.outerCutoff) 
	{
		vec3 	norm = normalize(Normal);
		float 	diff = max(dot(norm, lightDir), 0.0);
		vec3 	diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
			
		// specular
		vec3 	viewDir = normalize(viewPos - FragPos); // try with camera forward
		vec3 	reflectDir = reflect(-lightDir, norm);
		float 	spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 	specular = light.specular * spec * vec3(texture(material.specular, TexCoords));	
		
		
		diffuse *= attenuation;
		specular *= attenuation;
	
		float epsilon   = light.cutoff - light.outerCutoff;
		float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
		
		// we'll leave ambient unaffected so we always have a little light.
		diffuse  *= intensity;
		specular *= intensity;
		
		result = ambient + diffuse + specular;
	}
	else  // else, use ambient light so scene isn't completely dark outside the spotlight.
		result = ambient;
		
	return result;
}

//----------------------------------------------------------
void main()
{
    vec3 	norm = normalize(Normal);
    vec3 	viewDir = normalize(viewPos - FragPos);

    vec3 	result = CalcDirLight(dirLight, norm, viewDir);
	
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

	for(int i = 0; i < NR_SPOT_LIGHTS; i++)
		result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
	
	FragColor = vec4(result, 1.0);	
}