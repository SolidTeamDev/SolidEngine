#version 450

out vec4 FragColor;

in vec3 color;

uniform float RedMultiplier = 1.0f;
uniform float GreenMultiplier = 1.0f;
uniform float BlueMultiplier = 1.0f;

void main()
{
    float f = color.x/20.0f;
    if(f < 0.2f)
    f = 0.2f;
    if(f > 0.9f)
    f = 0.9f;
    float g = (1.0f-f);
    FragColor = vec4(g * RedMultiplier, g * GreenMultiplier, g * BlueMultiplier, 1);
}