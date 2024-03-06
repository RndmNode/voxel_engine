#shader vertex
#version 330 core

layout (location = 0) in vec4 a_Position;

// model view projection matrix
uniform mat4 u_MVP;
out vec3 color_coords;

void main()
{
   gl_Position = a_Position * u_MVP;
   color_coords = a_Position.xyz;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

in vec3 color_coords;

void main()
{
   color = vec4(color_coords.xy, 0.1, 1.0);
}