#version 460 core

out vec4 FragColor; 
in vec3 out_texCoord; 

uniform sampler2D uTexture; 

void main(void) 
{ 
    vec4 color = texture(uTexture, out_texCoord); 
    if(color.r < 0.1 || color.g < 0.1 || color.b < 0.1) 
        dicard; 

    FragColor = vec4(1.0 ,1.0, 1.0, 0.9); 
} 
