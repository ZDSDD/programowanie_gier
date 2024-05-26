#version 430 core

layout (location = 0) in vec3 Pos;
layout (location = 2) uniform mat4 u_ProjMatrix;
layout (location = 3) uniform mat4 u_ViewMatrix;
layout (location = 4) uniform mat4 u_ModelMatrix;

void main()
{
   gl_Position = u_ProjMatrix * u_ViewMatrix * u_ModelMatrix * vec4(Pos, 1.0);
}