#version 460 core

out vec4 FragColor;
uniform int uIsLight;

void main(void)
{
    if(uIsLight == 1)
        FragColor = vec4(vec3(1.0f), 1.0f);
    else
        FragColor = vec4(vec3(0.0f), 1.0f);
}

