#version 460 core 

in vec2 out_texCoord; 
in vec3 out_normal; 
in vec3 out_worldPosition; 

out vec4 FragColor; 

uniform sampler2D uTextures[4]; 
uniform float uHeightScale; 

// const float hRange1 = 0.1; 
// const float hRange2 = 0.3; 
// const float hRange3 = 0.5; 
// const float hRange4 = 0.75; 
    
uniform float hRange1; 
uniform float hRange2; 
uniform float hRange3; 
uniform float hRange4; 
    
uniform int uIsLightEnabled; 
uniform int uIsFogEnabled; 

vec4 uLightPosition; 
uniform vec3 uLightColor; 
uniform vec3 uViewPosition;

uniform float uFogStart; 
uniform float uFogEnd; 
uniform vec3 uFogColor; 

void main(void) 
{ 
    vec3 lightColor = vec3(1.0);
 
    /* -------- light color --------- */
    vec3 skyColor = vec3(0.7, 0.9, 0.9); 
    vec3 groundColor = vec3(0.4, 0.3, 0.2); 
    float mixFactor = dot(out_normal, vec3(0.0, 1.0, 0.0)) * 0.5 + 0.5; 
    vec3 ambientColor = mix(groundColor, skyColor, mixFactor); 

    lightColor = ambientColor; 

    if(uIsLightEnabled == 1) 
    {
        vec3 lightDirection = normalize(vec3(out_worldPosition - uLightPosition.xyz)); 
        float diffuse = max(0.0, dot(-lightDirection, out_normal)); 
        vec3 diffuseColor = diffuse * uLightColor; 

        float shininess = 32.0f; 
        vec3 reflectionVector = normalize(reflect(lightDirection, out_normal)); 
        vec3 viewDirection = normalize(vec3(uViewPosition - out_worldPosition)); 
        float specular = pow(max(0.0, dot(reflectionVector, viewDirection)), shininess); 
        vec3 specularColor = specular * uLightColor; 

        lightColor += (diffuseColor + specularColor); 
    } 

    /* -------- texture color --------- */ 
    float height = out_worldPosition.y/uHeightScale; 
    
    vec3 textureColor = vec3(0.0); 
    if(height < hRange1) 
    { 
        textureColor = texture(uTextures[0], out_texCoord).rgb; 
    } 
    else if(height < hRange2) 
    { 
        vec3 color0 = texture(uTextures[0], out_texCoord).rgb; 
        vec3 color1 = texture(uTextures[1], out_texCoord).rgb; 
        float delta = hRange2-hRange1; 
        float factor = (height - hRange1)/delta; 
        textureColor = mix(color0, color1, factor);  
    } 
    else if(height < hRange3) 
    { 
        vec3 color0 = texture(uTextures[1], out_texCoord).rgb; 
        vec3 color1 = texture(uTextures[2], out_texCoord).rgb; 
        float delta = hRange3-hRange2; 
        float factor = (height - hRange2)/delta; 
        textureColor = mix(color0, color1, factor); 
    } 
    else if(height < hRange4) 
    { 
        vec3 color0 = texture(uTextures[2], out_texCoord).rgb; 
        vec3 color1 = texture(uTextures[3], out_texCoord).rgb; 
        float delta = hRange4-hRange3; 
        float factor = (height - hRange3)/delta; 
        textureColor = mix(color0, color1, factor); 
    } 
    else 
    { 
        textureColor = texture(uTextures[3], out_texCoord).rgb; 
    } 

    /* ----- final color ----- */ 
    FragColor = vec4(textureColor * lightColor, 1.0f); 
} 
