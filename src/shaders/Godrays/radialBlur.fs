#version 460 core

in vec2 out_texCoord;

out vec4 FragColor;

uniform float uExposure;
uniform float uDecay;
uniform float uDensity;
uniform float uWeight;
uniform vec2 uLightPositionOnScreen;
uniform sampler2D uOcclusionTextureSampler;
uniform int uNumSamples;

void main(void)
{
	vec2 deltaTexCoord = vec2(out_texCoord - uLightPositionOnScreen);
	vec2 currentTexCoord = out_texCoord;
	deltaTexCoord *= 1.0 / float(uNumSamples) * uDensity;
	float illuminationDecay = 1.0f;

	vec4 color = vec4(0.0f);
	for(int i = 0; i < uNumSamples; ++i) {
		currentTexCoord -= deltaTexCoord;
		vec4 currentSample = texture(uOcclusionTextureSampler, currentTexCoord);
		currentSample *= illuminationDecay * uWeight;
	
		color = color + currentSample;
		illuminationDecay *= uDecay;
	}

	color *= uExposure;
	FragColor = vec4(color);
}

