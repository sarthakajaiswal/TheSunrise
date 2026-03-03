#version 460 core

in vec4 aPosition;
in vec2 aTexCoord;
out vec2 out_TexCoord;

void main(void) 
{
    gl_Position = aPosition;
    out_TexCoord = aTexCoord;
}

