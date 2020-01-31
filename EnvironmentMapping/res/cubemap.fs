#version 450 core

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform vec3 cameraPos;
uniform int isReflective;
uniform samplerCube skybox;
uniform samplerCube dynSkybox;
uniform sampler2D texture_diffuse1;
out vec4 FragColor;

bool isBlack(vec3 c)
{
	return (dot(c, c) == 0.0);
}

void main()
{    
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    if(isReflective == 1)
    {
	 	if(isBlack(texture(dynSkybox, R).rgb) )  	
	 		FragColor = vec4(texture(skybox, R).rgb, 1.0);
	 	else 
	 		FragColor = vec4(texture(dynSkybox, R).rgb, 1.0);
    }
    else 
    	FragColor = vec4(Position, 1.0);
}