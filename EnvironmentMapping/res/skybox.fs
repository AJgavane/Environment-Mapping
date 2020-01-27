#version 450 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform samplerCube skybox2;

void main()
{    
    FragColor = texture(skybox, TexCoords);
}