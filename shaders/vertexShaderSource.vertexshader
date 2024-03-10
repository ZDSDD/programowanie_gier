#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 TriangleColor;
// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main()
{
   gl_Position = MVP * vec4(aPos, 1.0);
   TriangleColor = aCol;
   
}