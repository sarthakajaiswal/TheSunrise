#version 460 core 
precision lowp int; 

in VS_OUT 
{ 
	vec3 color; 
	vec2 texCoord; 
	vec3 eyeDirection; 
	vec3 lightDirection; 
	vec3 viewSpaceNormal; 
} fs_in; 

uniform vec4 uLightPosition; 
uniform vec3 uLightAmbient; 
uniform vec3 uLightDiffuse; 
uniform vec3 uLightSpecular; 
uniform float uMaterialShininess; 

uniform vec3 uMaterialAmbient; 
uniform vec3 uMaterialDiffuse; 
uniform vec3 uMaterialSpecular; 

// normal mapping related 
uniform sampler2D uNormalMapSampler; 
uniform sampler2D uColorTextureSampler; 
uniform int uIsNormalMapEnabled = 1; 

uniform float uBlendStrength; 

out vec4 FragColor; 

void main(void) 
{ 
// normalize incoming L, V vectors
vec3 normalizedEyeDirection = normalize(fs_in.eyeDirection); 
vec3 normalizedLightDirection = normalize(fs_in.lightDirection); 

// if normal mapping enabled calculate normal using bump map else use normal passed as attribute 
vec3 normal; 
if(uIsNormalMapEnabled == 1) 
	normal = normalize(texture(uNormalMapSampler, fs_in.texCoord).rgb * 2.0 - vec3(1.0)); 
else 
	normal = fs_in.viewSpaceNormal; 

// calulate R (for specular) 
vec3 reflectionVector = reflect(-normalizedLightDirection, normal); 

// calculate the diffuse and specular contributions for each FRAGMENT 
vec3 texture_color = texture(uColorTextureSampler, fs_in.texCoord).rgb; 

vec3 ambient = texture_color * uLightAmbient * uMaterialAmbient; 
vec3 diffuse = max(dot(normal, normalizedLightDirection), 0.0) * texture_color; 
vec3 specular_albedo = uLightSpecular * uMaterialSpecular; 
vec3 specular = pow(max(dot(reflectionVector, normalizedEyeDirection), 0.0), uMaterialShininess) * specular_albedo; 

vec3 color = ambient + diffuse + specular; 

// use blending to add sparkle like highlight 
	float yellowMask = min(color.r, color.g) - color.b; 
	yellowMask = clamp(yellowMask*5.0, 0.0, 1.0); 
	vec3 blendedColor = mix(color.rgb, fs_in.color, yellowMask*uBlendStrength); 

FragColor = vec4(blendedColor, 1.0f); 
}; 


