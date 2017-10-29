#version 330 core
in vec2 TexCoords;

#ifdef OUTPUT_FLOAT
out float fragColor;
#else
out vec4 fragColor;
#endif

uniform sampler2D ssaoInput;

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x) 
    {
        for (int y = -2; y < 2; ++y) 
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(ssaoInput, TexCoords + offset).r;
        }
    }

#ifdef OUTPUT_FLOAT
    fragColor = result / (4.0 * 4.0);
#else
    fragColor = vec4(vec3(result / (4.0 * 4.0)), 1.0);
#endif
}
