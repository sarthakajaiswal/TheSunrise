#version 460 core

in vec2 out_TexCoord;

uniform sampler2D uTextureSampler; 

out vec4 FragColo

void main(void)
{
    FragColor = vec4(1.0, 0.0, 0.0, 1.0); texture(uTextureSampler, out_TexCoord);
} 

