#version 460 core

in vec4 aPosition;

uniform mat4 uRotationMatrix; 
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
out vec3 out_texCoords;

void main(void) 
{
    vec4 pos = uProjectionMatrix * mat4(mat3(uViewMatrix)) * uRotationMatrix * aPosition; 
    gl_Position = pos.xyww; 

    out_texCoords = vec3(aPosition.xyz);
} 

