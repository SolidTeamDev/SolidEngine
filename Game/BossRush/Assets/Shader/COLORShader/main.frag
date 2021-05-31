#version 450

out vec4 FragColor;

in vec3 color;
in vec3 pos;

float rand(vec3 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


void main()
{
    vec3 pos2 = vec3(pos.z, pos.y, pos.x);
    vec3 pos3 = vec3(pos.y, pos.z, pos.x);
    float f = color.x/20.0f;
    if(f < 0.2f)
    f = 0.2f;
    if(f > 0.9f)
    f = 0.9f;
    //FragColor = vec4(1.0f-f * rand(pos),1.0f-f* rand(pos2),1.0f-f* rand(pos3),1);
    FragColor = vec4(pos.x / 50.0, 0.0, pos.z / 50.0, 1);
}