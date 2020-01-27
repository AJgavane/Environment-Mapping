#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;	

out vec3 TexCoords;

void main()
{
    TexCoords = position;
    vec4 pos = u_projection * u_view * vec4(position, 1.0);
    gl_Position = pos.xyww;
}  