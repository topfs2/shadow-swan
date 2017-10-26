#version 330 core

#include "tonemap.glsl"

uniform vec3 lightColor;

out vec4 color;

void main()
{
    color = vec4(tonemapAuto(lightColor), 1.0f);
}
