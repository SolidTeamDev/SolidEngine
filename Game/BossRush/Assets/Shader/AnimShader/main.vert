#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 color;
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 weights;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

const int MAX_BONES = 230;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
uniform int boneIndex = 0;

out vec4 colorCoords;

void main()
{
    vec4 totalPosition = vec4(0,0,0,0.0f);
	colorCoords = vec4(color.x, 0, color.y, 1.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        
        if(boneIds[i] == -1)
            continue;
        if(boneIds[i] >= MAX_BONES)
        {
            totalPosition += vec4(pos,1.0f);
            break;
        }
        
        totalPosition +=  weights[i] * finalBonesMatrices[boneIds[i]] * vec4(pos,1.0f);
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * norm;
   }

    mat4 viewModel = view * model;
    gl_Position =  proj * (viewModel * vec4(totalPosition));
}

