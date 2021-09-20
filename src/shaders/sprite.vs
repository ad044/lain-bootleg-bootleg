#version 330 core
layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec2 a_TexCoord;
layout (location = 2) in float a_TexIndex;

out vec2 v_TexCoord;
out float v_TexIndex;

void main()
{
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    gl_Position = vec4(a_Pos, 0.0, 1.0);
}