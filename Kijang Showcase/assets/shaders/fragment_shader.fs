#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 diffuse_color;

void main()
{
    FragColor = vec4(diffuse_color * vec3(texture(texture_diffuse1, TexCoords)), 1.0);
}
