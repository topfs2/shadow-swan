#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D debugTexture;

void main()
{
    color = texture(debugTexture, TexCoords);
}
