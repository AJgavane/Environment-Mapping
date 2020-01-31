#version 450 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
uniform samplerCube dynSkybox;

void main()
{    
    FragColor = texture(skybox, TexCoords);// + texture(dynSkybox, TexCoords);
    // FragColor = texture(dynSkybox, TexCoords);// + texture(dynSkybox, TexCoords);
}