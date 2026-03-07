#version 460 core 

out vec4 FragColor;
in vec2 out_texCoord; 
uniform sampler2D uTexture; 
uniform float uAlpha; 

void main(void) 
{ 
    vec3 texColor = texture(uTexture, out_texCoord).rgb; 
    FragColor = vec4(texColor, 0.8); 
}



