#version 460 core

in vec2 out_texCoord;
out vec4 FragColor;

uniform sampler2D uObjectsTexture; 
uniform sampler2D uLightTexture; 

void main(void)
{
    vec4 objectsColor = vec4(0.0f); 
    vec4 lightColor = vec4(0.0f); 

    lightColor = texture(uLightTexture, out_texCoord); 

    objectsColor = texture(uObjectsTexture, out_texCoord); 

    // apply light color to output 
    vec4 finalColor = objectsColor; 

    // // override the light color with object color wherever object exists  
    // objectsColor = texture(uObjectsTexture, out_texCoord); 
    // if(objectsColor.rgb != vec3(0.0)) 
    //     finalColor = objectsColor; 

    FragColor = vec4(1.0, 0.0, 0.0, 1.0); 
}

