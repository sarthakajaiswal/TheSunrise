#version 460 core 

in vec4 aPosition; 
in vec2 aTexCoord; 

out vec2 out_texCoord; 

void main(void)  
{ 
    out_texCoord = aTexCoord; 
    gl_Position = aPosition; 
}
