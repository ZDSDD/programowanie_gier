#version 460 core
out vec4 FragColor;
in vec3 TriangleColor;
void main()
{
   FragColor = vec4(TriangleColor, 1.0f);
}