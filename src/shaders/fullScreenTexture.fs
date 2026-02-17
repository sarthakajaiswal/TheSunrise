#version 460 core 

out vec4 FragColor;
in vec2 out_texCoord; 
uniform sampler2D uTexture; 

void main(void) 
{ 
    FragColor = texture(uTexture, out_texCoord); 
}



