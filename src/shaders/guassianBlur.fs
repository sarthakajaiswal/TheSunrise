#version 460 core 

in vec2 out_texCoord; 

uniform sampler2D uTextureSampler; 
uniform int uHorizontalOrVertical; 

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216); 

out vec4 FragColor; 

void main(void) 
{ 
	vec2 tex_offset = 1.0 / textureSize(uTextureSampler, 0); 
	vec3 result = texture(uTextureSampler, out_texCoord).rgb * weight[0]; 

	if(uHorizontalOrVertical == 0) 
	{ 
		for(int i = 1; i < 5; ++i) 
		{ 
			result += texture(uTextureSampler, out_texCoord + vec2(tex_offset.x*i, 0.0)).rgb * weight[i]; 
			result += texture(uTextureSampler, out_texCoord - vec2(tex_offset.x*i, 0.0)).rgb * weight[i]; 
		} 
	} 
	else 
	{ 
		for(int i = 1; i < 5; ++i) 
		{ 
			result += texture(uTextureSampler, out_texCoord + vec2(0.0, tex_offset.y*i)).rgb * weight[i]; 
			result += texture(uTextureSampler, out_texCoord - vec2(0.0, tex_offset.y*i)).rgb * weight[i]; 
		} 
	} 

	FragColor = vec4(result, 1.0); 
}

