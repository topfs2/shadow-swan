#version 330 core

uniform bool useNormalMapping;

struct Material {
    sampler2D albedoSampler;
    sampler2D metalSampler;
    sampler2D roughnessSampler;
    sampler2D occlusionSampler;
    sampler2D emissiveSampler;
    sampler2D normalSampler;
};

uniform Material material;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
    vec3 Normal;
} fs_in;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 Normal;

void main()
{
    gPosition.rgb = fs_in.FragPos;

    if (useNormalMapping) {
        Normal = texture(material.normalSampler, fs_in.TexCoords).rgb;
        Normal = normalize(Normal * 2.0 - 1.0);
        Normal = normalize(fs_in.TBN * Normal);
    } else {
        Normal = normalize(fs_in.Normal);
    }
}
