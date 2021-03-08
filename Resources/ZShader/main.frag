#version 450

out vec4 FragColor;

in vec3 color;

void main()
{
    float f = color.x/20.0f;
    if(f < 0.2f)
    f = 0.2f;
    if(f > 0.9f)
    f = 0.9f;
    FragColor = vec4(1.0f-f,1.0f-f,1.0f-f,1);
}