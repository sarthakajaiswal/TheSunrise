#version 460 core

in vec2 out_texCoord;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform float uTex1Factor; 
uniform float uTex2Factor; 

out vec4 FragColor;

void main(void)
{
	const float gamma = 2.2;
	
	vec3 tex1Color = texture(uTexture1, out_texCoord).rgb * uTex1Factor;
	vec3 tex2Color = texture(uTexture2, out_texCoord).rgb * uTex2Factor;

	// additive blending 
	vec3 result = tex1Color + tex2Color;

	// // tone mapping 
    // vec3 result = vec3(1.0) - exp(-tex2Color * uExposure);
	// // gamma correction 
    // result = pow(result, vec3(1.0/gamma)); 

	FragColor = vec4(result, 1.0);
}

