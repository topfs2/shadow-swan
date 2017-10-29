#version 330 core

#include "tonemap.glsl"

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D colorInput;
uniform sampler2D brightInput;
uniform bool bloom;

uniform sampler2D ssaoSampler;
uniform bool useSSAO;

void main()
{
    vec3 hdrColor = texture(colorInput, TexCoords).rgb;
    vec3 bloomColor = texture(brightInput, TexCoords).rgb;

    // Lets just multiply the LDR scene with the SSAO, it really should be applied to the ambient parts only, but needs a deferred pipeline for that
    if (useSSAO) {
        hdrColor *= texture(ssaoSampler, TexCoords).r;
    }

#if DEBUG
    vec3 result = bloom ? bloomColor : hdrColor;
#else
    vec3 result = hdrColor;
    if (bloom) {
        result += bloomColor; // additive blending
    }
#endif

    result = tonemapAuto(result);
    FragColor = vec4(result, 1.0);
}
