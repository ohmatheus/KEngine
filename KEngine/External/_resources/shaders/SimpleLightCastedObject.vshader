#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 TexCoords;

void main()
{
    gl_Position = proj * view * model * vec4(aPos, 1.0);
	
	// normal matrix is defined as 'the transpose of the inverse of the upper-left 3x3 part of the model matrix'.
	//Phew, that's a mouthful and if you don't really understand what that means, don't worry; we haven't discussed inverse and transpose matrices yet. 
	// Note that most resources define the normal matrix as derived from the model-view matrix, but since we're working in world space (and not in view space) we will derive it from the model matrix.

	//In the vertex shader we can generate the normal matrix by using the inverse and transpose functions in the vertex shader that work on any matrix type. 
	// Note that we cast the matrix to a 3x3 matrix to ensure it loses its translation properties and that it can multiply with the vec3 normal vector:
	// Normal = mat3(transpose(inverse(model))) * aNormal;
	
	Normal = aNormal;
	FragPos = vec3(model * vec4(aPos, 1.0));
	TexCoords = aTexCoords;
} 