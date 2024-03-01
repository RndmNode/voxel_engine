#shader vertex
#version 330 core

// take in position and color from array
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 Color;

// output color info to fragment shader
out vec4 v_Color;

void main()
{
    v_Color = Color;
    gl_Position = position;
}

#shader fragment
#version 330 core

// Set output variable
layout (location = 0) out vec4 color;

// take in color from vertex shader output
in vec4 v_Color;

void main()
{
    color = v_Color;
}