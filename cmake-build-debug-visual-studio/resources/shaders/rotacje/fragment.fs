#version 430 core

layout (location = 1) uniform vec4 TriangleColor;

out vec4 FragColor;
void main()
{
   FragColor = TriangleColor;
}