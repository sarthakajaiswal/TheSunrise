#version 460 core

in vec4 aPosition;
in vec2 aTexCoord; 
out vec3 out_texCoord; 

uniform mat4 uMVPMatrix;

void main(void) 
{
    out_texCoord = aTexCoord; 
    gl_Position = uMVPMatrix * aPosition;
}
