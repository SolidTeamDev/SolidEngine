#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
    gl_Position = proj * view * model * vec4(inPos, 1.0f);
}