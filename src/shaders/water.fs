#version 460 core

in vec2 out_texCoord;
in vec3 out_toCameraVector;
in vec3 out_fromLightVector;

// in float out_cameraToWaterDistance;
out vec4 FragColor;

uniform sampler2D uReflectionTexture;
uniform sampler2D uRefractionTexture;
uniform sampler2D uDuDvTexture;
uniform sampler2D uNormalMap;
uniform sampler2D uDepthMap;
uniform vec3 uLightColor;

uniform vec2 uScreenSize;

uniform float uMoveFactor;
const float waveStrength = 0.02;
const float shineDamper = 20.0;
const float reflectivity = 0.6;

void main(void)
{
    vec2 ndc = gl_FragCoord.xy/uScreenSize;
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    vec2 reflectTexCoords = vec2(ndc.x, 1.0-ndc.y);

    vec2 distortedTexCoords = texture(uDuDvTexture, vec2(out_texCoord.x + uMoveFactor, out_texCoord.y)).rg * 0.1;
    distortedTexCoords = out_texCoord + vec2(distortedTexCoords.x, distortedTexCoords.y+uMoveFactor);
    vec2 distortion = (texture(uDuDvTexture, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;

    reflectTexCoords += distortion;
    reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
    reflectTexCoords.y = clamp(reflectTexCoords.y, 0.001, 0.999);

    refractTexCoords += distortion;
    refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

    vec4 reflectColor = texture(uReflectionTexture, reflectTexCoords);
    vec4 refractColor = texture(uRefractionTexture, refractTexCoords);

    vec3 viewVector = normalize(out_toCameraVector);
    float refractiveIndex = dot(viewVector, vec3(0.0, 1.0, 0.0));
    refractiveIndex = pow(refractiveIndex, 2.0);

    vec4 normalMapColor = texture(uNormalMap, distortedTexCoords);
    vec3 normal = vec3(normalMapColor.r * 2.0 - 1.0, normalMapColor.b, normalMapColor.g * 2.0 - 1.0);
    normal = normalize(normal);

    vec3 reflectedLight = reflect(normalize(out_fromLightVector), normal);
    float specular = max(dot(reflectedLight, viewVector), 0.0);
    specular = pow(specular, shineDamper);
    vec3 specularHighlights = uLightColor * specular * reflectivity;

    FragColor = mix(reflectColor, refractColor, refractiveIndex) + vec4(specularHighlights, 0.0);
}

