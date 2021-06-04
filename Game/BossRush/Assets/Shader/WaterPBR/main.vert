#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;

out vec2 vUV;
out vec3 vWorldPos;
out vec3 vNormal;
out float time;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;
uniform float _GlobalTime;

void main()
{
    vUV = inUV;
    vWorldPos = vec3(model * vec4(inPos,1.0));
    vNormal = mat3(model) * inNormal;
    time = _GlobalTime;
    gl_Position = proj * view * vec4(vWorldPos, 1.0f);
}