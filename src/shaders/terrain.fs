#version 460 core

in vec2 out_texCoord;
in vec3 out_normal; 
in float out_height; 

uniform sampler2D uTextureSampler0;
uniform sampler2D uTextureSampler1;
uniform sampler2D uTextureSampler2;
uniform sampler2D uTextureSampler3;

uniform vec3 uLightDirection; 

uniform float uHeight0; 
uniform float uHeight1; 
uniform float uHeight2; 
uniform float uHeight3; 

out vec4 FragColor;

vec4 calcTexColor() 
{
    vec4 texColor; 

    float height = out_height; 

    if(height < uHeight0) 
        texColor = texture(uTextureSampler0, out_texCoord); 
    else if(height < uHeight1) 
    {
        vec4 color0 = texture(uTextureSampler0, out_texCoord); 
        vec4 color1 = texture(uTextureSampler1, out_texCoord); 
        float delta = uHeight1 - uHeight0; 
        float factor = (height-uHeight0) / delta; 
        texColor = mix(color0, color1, factor); 
    } 
    else if(height < uHeight2) 
    {
        vec4 color0 = texture(uTextureSampler1, out_texCoord); 
        vec4 color1 = texture(uTextureSampler2, out_texCoord); 
        float delta = uHeight2 - uHeight1; 
        float factor = (height-uHeight1) / delta; 
        texColor = mix(color0, color1, factor); 
    } 
    else if(height < uHeight3) 
    {
        vec4 color0 = texture(uTextureSampler2, out_texCoord); 
        vec4 color1 = texture(uTextureSampler3, out_texCoord); 
        float delta = uHeight3 - uHeight2; 
        float factor = (height-uHeight2) / delta; 
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

     