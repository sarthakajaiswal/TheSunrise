#version 460 core

in vec2 out_texCoord;
in vec3 out_normal; 
in float out_height; 

uniform sampler2D uTextureSampler0;
uniform sampler2D uTextureSampler1;
uniform sampler2D uTextureSampler2;
uniform sampler2D uTextureSampler3;

uniform vec3 uLightDirection; 

float height0 = 20.0; 
float height1 = 50.0; 
float height2 = 100.0; 
float height3 = 150.0; 

out vec4 FragColor;

vec4 calcTexColor() 
{
    vec4 texColor; 

    float height = out_height; 

    if(height < height0) 
        texColor = texture(uTextureSampler0, out_texCoord); 
    else if(height < height1) 
    {
        vec4 color0 = texture(uTextureSampler0, out_texCoord); 
        vec4 color1 = texture(uTextureSampler1, out_texCoord); 
        float delta = height1 - height0; 
        float factor = (height-height0) / delta; 
        texColor = mix(color0, color1, factor); 
    } 
    else if(height < height2) 
    {
        vec4 color0 = texture(uTextureSampler1, out_texCoord); 
        vec4 color1 = texture(uTextureSampler2, out_texCoord); 
        float delta = height2 - height1; 
        float factor = (height-height1) / delta; 
        texColor = mix(color0, color1, factor); 
    } 
    else if(height < height3) 
    {
        vec4 color0 = texture(uTextureSampler2, out_texCoord); 
        vec4 color1 = texture(uTextureSampler3, out_texCoord); 
        float delta = height3 - height2; 
        float factor = (height-height2) / delta; 
        texColor = mix(color0, color1, factor);    
    } 
    else 
    {
        texColor = texture(uTextureSampler3, out_texCoord); 
    } 

    return (texColor); 
} 

void main(void)
{
    vec3 normal = normalize(out_normal); 
    float diffuse = max(0.2, dot(normal, -normalize(uLightDirection))); 
    FragColor = calcTexColor() * diffuse;
}

     