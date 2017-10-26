#version 330 core

#include "tonemap.glsl"

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D colorInput;
uniform sampler2D brightInput;
uniform bool bloom;

void main()
{
    vec3 hdrColor = texture(colorInput, TexCoords).rgb;
    vec3 bloomColor = texture(brightInput, TexCoords).rgb;

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
