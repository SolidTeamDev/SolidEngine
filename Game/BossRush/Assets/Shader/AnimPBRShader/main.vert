#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 weights;

out vec2 vUV;
out vec3 vWorldPos;
out vec3 vNormal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

const int MAX_BONES = 230;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
uniform int boneIndex = 0;

vec4 animProcess()
{
	vec4 totalPosition = vec4(0,0,0,0.0f);
	vec3 totalNorm = vec3(0,0,0);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        
        if(boneIds[i] == -1)
            continue;
        if(boneIds[i] >= MAX_BONES)
        {
            totalPosition += vec4(inPos,1.0f);
            break;
        }
        
        totalPosition +=  weights[i] * finalBonesMatrices[boneIds[i]] * vec4(inPos,1.0f);
        totalNorm +=  mat3(finalBonesMatrices[boneIds[i]]) * inNormal;
   }

    mat4 viewModel = view * model;
    vNormal =  mat3(model) *totalNorm;
    return proj * (viewModel * vec4(totalPosition));
}

void main()
{
    vUV = inUV;
    vWorldPos = vec3(model * vec4(inPos,1.0));
    //vNormal = mat3(model) * inNormal;

    gl_Position = animProcess();
}

