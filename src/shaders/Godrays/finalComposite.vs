#version 460 core 

in vec2 aPosition; 
in vec2 aTexCoord; 

out vec2 out_texCoord; 
	
void main(void)  
{ 
    out_texCoord = aTexCoord; 
    gl_Position = vec4(aPosition, 0.0f, 1.0f); 
}

