#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;


uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;


out vec3 Normal;
out vec3 Position;

void main()
{
	Normal =  normalize(transpose(inverse(mat3(u_model))) * normal);
    Position = vec3(u_model * vec4(position, 1.0));
    gl_Position = u_projection * u_view * u_model * vec4(Position, 1.0);
}