#version 460 core 

in vec2 out_TexCoord; 
uniform sampler2D uHDRTextureSampler; 
uniform float uExposure; 
	
out vec4 FragColor; 
	
void main(void) 
{ 
	const float gamma = 2.2; 

	vec3 hdrColor = texture(uHDRTextureSampler, out_TexCoord).rgb; 

    // exposure tone-mapping 
	vec3 mapped = vec3(1.0) - exp(-hdrColor * uExposure); 

	FragColor = vec4(mapped, 1.0f); 
}
