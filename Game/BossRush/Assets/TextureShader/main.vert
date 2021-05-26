#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTex;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoord;

void main()
{
    gl_Position = proj * view * model * vec4(inPos, 1.0f);
    TexCoord = inTex;
}