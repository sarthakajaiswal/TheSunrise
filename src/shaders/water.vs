#version 460 core 
	
in vec4 aPosition; 

// out float out_cameraToWaterDistance; 

out vec2 out_texCoord; 
uniform mat4 uModelMatrix; 
uniform mat4 uViewMatrix; 
uniform mat4 uProjectionMatrix; 

uniform vec3 uCameraPosition; 
uniform vec3 uLightPosition; 

out vec3 out_toCameraVector; 
out vec3 out_fromLightVector; 

const float tiling = 1.0f; 

void main(void)  
{ 
	gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(aPosition.xyz, 1.0); 

	vec4 worldPosition = uModelMatrix * aPosition; 
	out_toCameraVector = uCameraPosition - worldPosition.xyz; 
	out_texCoord = vec2(aPosition.x/2.0+0.5, aPosition.z/2.0+0.5) * tiling; 
	out_fromLightVector = worldPosition.xyz - uLightPosition; 

	vec4 clipPos = gl_Position; 
	// out_cameraToWaterDistance = clipPos.z/clipPos.w; 
} 

