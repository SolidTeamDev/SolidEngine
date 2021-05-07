#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

out vec2 vUV;
out vec3 vWorldPos;
out vec3 vNormal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vUV = inUV;
    vWorldPos = vec3(model * vec4(inPos,1.0));
    gl_Position = projection * view * model * vec4(inPos, 1.0f);
}