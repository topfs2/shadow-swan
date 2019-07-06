#version 330 core
layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoords;

uniform sampler2D debugTexture;

void main()
{
    gl_Position = vec4(position.x, position.z, 0.0f, 1.0f);
}
