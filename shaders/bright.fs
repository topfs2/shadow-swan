#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D inputSampler;
const float minBright = 0.7;

void main()
{
    color = max(vec4(0.0), (texture(inputSampler, TexCoords) - minBright));
}
