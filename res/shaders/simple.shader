#shader vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Offset;

// model view projection matrices
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec4 color_coords;

void main()
{
   // save position in editable variable
   vec3 pos = a_Position;

   if (a_Offset.w == 1)    // Back Face
   {
      pos.xy = pos.yx;
      pos.z--;
   }
   else if (a_Offset.w == 2)   // Left Face
   {
      pos.xz = pos.zx;
      pos.z--;
   }
   else if (a_Offset.w == 3)   // Right Face
   {
      pos.xz = pos.zx;
      pos.yz = pos.zy;
      pos.x++;
      pos.z--;
   }
   else if (a_Offset.w == 4)   // Top Face
   {
      pos.yz = pos.zy;
      pos.xz = pos.zx;
      pos.y++;
      pos.z--;
   }
   else if (a_Offset.w == 5)   // Bottom Face
   {
      pos.zy = pos.yz;
      pos.z--;
   }

   gl_Position = u_Projection * u_View * u_Model * (vec4(pos, 1.0) + vec4(a_Offset.xyz, 0.0));
   color_coords = vec4(a_Position, 1.0);
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

in vec4 color_coords;

void main()
{
   // color = vec4(0.5, 0.5, 0.5, 1.0);
   color = color_coords;
}