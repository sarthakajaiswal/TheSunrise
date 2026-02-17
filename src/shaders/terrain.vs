#version 460 core

in vec4 aPosition;
in vec2 aTexCoord;
in vec3 aNormal; 

uniform mat4 uMVPMatrix;

out vec2 out_texCoord;
out vec3 out_normal; 
out float out_height; 

void main(void) 
{
    gl_Position = uMVPMatrix * aPosition;
    out_texCoord = aTexCoord;
    out_normal = aNormal; 
    out_height = aPosition.y; 
}

