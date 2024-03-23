#shader vertex
#version 330 core

layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec3 a_Offset;

// model view projection matrices
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 color_coords;

void main()
{
   gl_Position = u_Projection * u_View * u_Model * (a_Position + vec4(a_Offset, 0.0));
   color_coords = a_Position;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

in vec4 color_coords;

void main()
{
   color = vec4(0.5, 0.5, 0.5, 1.0);
}