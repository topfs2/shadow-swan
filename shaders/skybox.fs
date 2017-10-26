#version 330 core

#include "tonemap.glsl"

in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube skybox;


void main()
{
    vec3 color = texture(skybox, TexCoords).rgb;
    color = tonemapAuto(color);

    FragColor = vec4(color, 1.0);
}
