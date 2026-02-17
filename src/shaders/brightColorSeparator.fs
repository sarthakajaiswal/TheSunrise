#version 460 core 

in vec2 out_texCoord; 

uniform sampler2D uTexture; 
out vec4 FragColor; 

void main(void) 
{ 
	vec4 textureColor = texture(uTexture, out_texCoord); 

	float brightness = dot(textureColor.rgb, vec3(0.2126, 0.7152, 0.122)); 
	if(brightness > 1.0) 
		FragColor = textureColor; 
	else 
		FragColor = vec4(0.0, 0.0, 0.0, 1.0); 
}


