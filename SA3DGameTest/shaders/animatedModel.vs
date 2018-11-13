#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3  normal;
layout (location = 2) in vec2 texCoords;
layout (location = 5) in ivec4 BoneIDs;
layout (location = 6) in vec4 Weights;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

const int MAX_BONES = 100;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 gBones[MAX_BONES];

void main()
{
	mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
	BoneTransform += gBones[BoneIDs[1]] * Weights[1];
    BoneTransform += gBones[BoneIDs[2]] * Weights[2];
    BoneTransform += gBones[BoneIDs[3]] * Weights[3];

	vec4 PosL = BoneTransform * vec4(position, 1.0);


	FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal; 
    TexCoords = texCoords;    
	gl_Position = projection * view * model * PosL;
    
}