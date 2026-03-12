#version 460 core

in vec2 out_texCoord;
out vec4 FragColor;

layout(binding=0) uniform sampler2D uObjectsTexture; 
layout(binding=1) uniform sampler2D uObjectDepthTexture; 
layout(binding=2) uniform sampler2D uLightTexture; 
layout(binding=3) uniform sampler2D uLightDepthTexture; 

void main(void)
{
    // vec4 objectsColor = vec4(0.0f); 
    vec4 lightColor = vec4(0.0f); 

    lightColor = texture(uLightTexture, out_texCoord); 
    // // converts light objects into white 
    // if(objectsColor.rgb != vec3(0.0)) 
    //     objectsColor = vec4(1.0, 1.0, 1.0, 1.0); 

    // // apply light color (white) to output 
    // vec4 finalColor = lightColor; 

    // // override the white color with black whereever the object exists 
    // objectsColor = texture(uObjectsTexture, out_texCoord); 
    // if(objectsColor.rgb != vec3(0.0)) 
    //     finalColor = vec4(0.0, 0.0, 0.0, 1.0); 

    // FragColor = finalColor; // vec4(1.0, 0.0, 0.0, 1.0); 

    float lightDepth = texture(uLightDepthTexture, out_texCoord).r; 
    float objectDepth = texture(uObjectDepthTexture, out_texCoord).r; 

    FragColor = vec4(0.0, 0.0, 0.0, 1.0); 

    if(objectDepth <= lightDepth)
    {
        // FragColor *= lightColor; 
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);  
    } 
    else 
        // FragColor += lightColor; 
        FragColor = vec4(1.0, 1.0, 1.0, 1.0); 
}

