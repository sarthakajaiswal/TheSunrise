#version 460 core 

out vec4 FragColor; 

in vec2 out_texCoord; 
	
uniform sampler2D uSceneTexture; 
uniform sampler2D uGodRaysTexture; 
uniform float uGodRaysStrength; 
	
void main() 
{ 
	/* sample both textures */
    vec3 sceneColor = texture(uSceneTexture, out_texCoord).rgb; 
    vec3 godRays = texture(uGodRaysTexture, out_texCoord).rgb; 
		
	/* addittive blending */
    vec3 finalColor = sceneColor + godRays * uGodRaysStrength; 
    FragColor = vec4(finalColor, 1.0); 
}
