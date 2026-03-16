#version 460 core 

out vec4 FragColor;
in vec2 out_texCoord; 
uniform sampler2D uTexture; 
uniform float uAlpha; 

void main(void) 
{ 
    vec4 texColor = texture(uTexture, out_texCoord); 
    if(texColor.a < 0.01) 
        discard; 
    FragColor = vec4(texColor.rgb, uAlpha); 
}



