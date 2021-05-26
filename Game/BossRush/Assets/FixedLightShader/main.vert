#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;


out vec3 pos;
out vec3 normal;
out vec2 texCoord;


void main()
{
    gl_Position = proj * view * model * vec4(inPos, 1.0f);
    pos = (model * vec4(inPos, 1.0f)).xyz;
    normal = mat3(transpose(inverse(model))) * inNormal;
    //pos = gl_Position.xyz;
    texCoord = inTexCoord;
}