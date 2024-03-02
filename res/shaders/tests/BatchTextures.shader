#shader vertex
#version 330 core

// take in position and color from array
layout (location = 0) in vec4 a_Position;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoords;
layout (location = 3) in float a_TextureMap;

// output color and texture info to fragment shader
out vec4 v_Color;
out vec2 v_TexCoords;
out float v_TexIndex;

void main()
{
    v_Color = a_Color;
    gl_Position = a_Position;
    v_TexCoords = a_TexCoords;
    v_TexIndex = a_TextureMap;
}

#shader fragment
#version 330 core

// Set output variable
layout (location = 0) out vec4 color;

// take in color and texture from vertex shader output
in vec4 v_Color;
in vec2 v_TexCoords;
in float v_TexIndex;

uniform sampler2D u_Textures[2];

void main()
{
    int index = int(v_TexIndex);
    color = texture(u_Textures[index], v_TexCoords);
}