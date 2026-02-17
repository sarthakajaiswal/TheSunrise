#version 460 core 
precision lowp int; 

in vec4 aPosition; 
in vec3 aColor; 
in vec2 aTexCoord; 
in vec3 aNormal; 
in vec3 aTangent; 

out VS_OUT 
{ 
	vec3 color; 
	vec2 texCoord; 
	vec3 eyeDirection; 
	vec3 lightDirection; 
	vec3 viewSpaceNormal; 
} vs_out; 

uniform mat4 uModelViewMatrix; 
uniform mat4 uProjectionMatrix; 

uniform vec4 uLightPosition; 
uniform int uIsNormalMapEnabled; 

void main(void) 
{ 
    // calculate vertex position in view space 
    vec4 viewSpacePosition = uModelViewMatrix * aPosition; 

    vs_out.eyeDirection = -viewSpacePosition.xyz;
    vs_out.lightDirection = uLightPosition.xyz - viewSpacePosition.xyz;

    // if normal mapping then calculate viewDirection and lightDirection using TBN 
    if(uIsNormalMapEnabled == 1) 
    { 
        // calculate normal (N) and tanget (T) vectors in view space from incoming ovject space vectors 
        vec3 N = normalize(mat3(uModelViewMatrix) * aNormal); 
        vec3 T = normalize(mat3(uModelViewMatrix) * aTangent); 
        vec3 B = cross(N, T); 

        // light vector (L) is vector from point of intersection to the light. 
        // calulate that and multiply it by TBN Matrix 
        vec3 L = vs_out.lightDirection; 
        vs_out.lightDirection = normalize(vec3(dot(L, T), dot(L, B), dot(L, N))); 

        // the view vector is vector from point of intersection to viewer, which is in view space is simply 
        // the negative of the position 
        // calculate that and multiply by TBN matrix 
        vec3 V = vs_out.eyeDirection; 
        vs_out.eyeDirection = normalize(vec3(dot(V, T), dot(V, B), dot(V, N))); 
    } 

    // calculate normal in view space 
    vs_out.viewSpaceNormal = mat3(uModelViewMatrix) * aNormal;

    vs_out.texCoord = aTexCoord; 
    vs_out.color = aColor; 

    // calculate clip-space position of each vertex
    gl_Position = uProjectionMatrix * viewSpacePosition;
}; 

