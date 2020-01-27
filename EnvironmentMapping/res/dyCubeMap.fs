#version 450 core

layout (location = 0) out vec4 color;

in vec2 TexCoords;
in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform sampler2D texture_diffuse1;


void main()
{
	 vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    // color = vec4(texture(skybox, R).rgb, 1.0);

	color =  vec4(1.0, 0.0, 0.0, 1.0);//texture(texture_diffuse1, TexCoords);
}