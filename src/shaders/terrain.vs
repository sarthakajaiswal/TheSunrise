#version 460 core 

in vec4 aPosition; 
in vec2 aTexCoord; 
in vec3 aNormal; 

out vec2 out_texCoord; 
out vec3 out_normal; 
out vec3 out_worldPosition; 

uniform mat4 uModelMatrix; 
uniform mat4 uViewMatrix; 
uniform mat4 uProjectionMatrix; 
uniform mat4 uNormalMatrix; 

void main(void)  
{ 
   gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition; 
   out_worldPosition = (uModelMatrix * aPosition).xyz; 
   out_texCoord = aTexCoord; 
   out_normal = (mat3(transpose(inverse(uModelMatrix))) * aNormal).xyz; 
   out_normal = normalize(out_normal); 
} 

