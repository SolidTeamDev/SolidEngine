#version 450
in vec2 TexCoord;

out vec4 FragColor;
uniform sampler2D Diffuse;
void main()
{
    FragColor = texture(Diffuse, TexCoord);
}