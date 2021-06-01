#version 450

in vec4 colorCoords;

out vec4 fragColor;

void main()
{
    fragColor = colorCoords;
}