#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
    vec3 Normal;
} vs_out;

void main()
{
    vs_out.FragPos = vec3(view * model * vec4(position, 1.0f));
    vs_out.TexCoords = vec2(texCoord.x, 1.0 - texCoord.y);

    vec3 T = normalize(vec3(view * model * vec4(tangent,   0.0)));
    vec3 B = normalize(vec3(view * model * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(view * model * vec4(normal,    0.0)));

    vs_out.TBN = mat3(T, B, N);
    vs_out.Normal = mat3(transpose(inverse(view * model))) * normal;

    gl_Position = projection * view *  model * vec4(position, 1.0f);
}
