#shader vertex
#version 330 core

layout (location = 0) in vec4 a_Position;
// layout (location = 1) in vec2 a_TexCoord;

// model view projection matrix
// uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 color_coords;
out vec2 tex_coords;

void main()
{
   gl_Position = u_Projection * u_View * u_Model * a_Position;
   // tex_coords = a_TexCoord;
   color_coords = a_Position;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

in vec4 color_coords;
// in vec2 tex_coords;

// uniform sampler2D u_Texture;

void main()
{
   // vec4 texColor = texture(u_Texture, tex_coords);
   color = vec4(0.8, 0.8, 0.8, 1.0);
}