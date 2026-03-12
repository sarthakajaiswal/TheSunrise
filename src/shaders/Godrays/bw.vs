#version 460 core

in vec4 aPosition;
in vec3 aColor; 
out vec3 out_color; 

uniform mat4 uMVPMatrix;

void main(void) 
{
    out_color = aColor; 
    gl_Position = uMVPMatrix * aPosition;
}
