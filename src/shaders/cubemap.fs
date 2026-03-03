#version 460 core

in vec3 out_texCoords;
out vec4 FragColor; 
uniform samplerCube uSkyboxSampler;

void main(void)
{
    FragColor = texture(uSkyboxSampler, out_texCoords);
}
