#version 410 core

struct Material
{
	sampler2D 				texture_diffuse1;
};

uniform Material 			material;
in vec2 					TexCoords;
out 	vec4 				FragColor;

void main()
{
	vec4	texColor = vec4(texture(material.texture_diffuse1, TexCoords));
	
	FragColor = texColor;
}