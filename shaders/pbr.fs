#version 330 core

#include "tonemap.glsl"

struct Material {
    sampler2D albedoSampler;
    sampler2D metalSampler;
    sampler2D roughnessSampler;
    sampler2D AOSampler;
    sampler2D emissiveSampler;
    sampler2D normalSampler;
};

uniform samplerCube skyboxSampler;
uniform samplerCube irradianceSampler;
uniform samplerCube radianceSampler;
uniform sampler2D brdfSampler;

uniform bool useIBL;
uniform bool useCheapIBL;

uniform Material material;
uniform vec3 viewPos;

#ifdef MAX_LIGHTS
uniform int lights;
uniform vec3 lightPositions[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS];
#endif

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
#ifdef HAVE_NORMAL_SAMPLER
    mat3 TBN;
#else
    vec3 Normal;
#endif
} fs_in;

layout (location = 0) out vec4 FragColor;


const float PI = 3.14159265359;
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 sRGB_linear(vec3 rgb)
{
    return pow(rgb, vec3(2.2));
}

vec3 linear_sRGB(vec3 linear)
{
    return pow(linear, vec3(1.0/2.2));
}

void main()
{
#ifdef HAVE_NORMAL_SAMPLER
    vec3 Normal = texture(material.normalSampler, fs_in.TexCoords).rgb;
    Normal = normalize(Normal * 2.0 - 1.0);
    Normal = normalize(fs_in.TBN * Normal);
#else
    vec3 Normal = normalize(fs_in.Normal);
#endif

    vec3 albedo = texture(material.albedoSampler, fs_in.TexCoords).rgb;
    float ao = texture(material.AOSampler, fs_in.TexCoords).r;
    float metallic = texture(material.metalSampler, fs_in.TexCoords).b;
    float roughness = texture(material.roughnessSampler, fs_in.TexCoords).g;

#ifdef HAVE_EMISSIVE_SAMPLER
    vec3 emissive = texture(material.emissiveSampler, fs_in.TexCoords).rgb;
#ifdef HAVE_EMISSIVE_SRGB
    emissive = sRGB_linear(emissive);
#endif
#endif

    vec3 WorldPos = fs_in.FragPos;

    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - WorldPos);
    vec3 R = -normalize(reflect(V, N));

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0);

#ifdef MAX_LIGHTS
    for (int i = 0; i < lights; i++) {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 nominator    = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;

        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }
#endif

    vec3 ambient = vec3(0);
    if (useIBL) {
        vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 irradiance = vec3(0);
        if (useCheapIBL) {
            irradiance = texture(skyboxSampler, N, 8).rgb;
        } else {
            irradiance = texture(irradianceSampler, N).rgb;
        }

#ifdef HAVE_IBL_SRGB
        irradiance = sRGB_linear(irradiance); // TODO remove when we have linear HDR env map
#endif
        vec3 diffuse    = irradiance * albedo;

        const float MAX_REFLECTION_LOD = 7.0;
        vec3 prefilteredColor = vec3(0);

        if (useCheapIBL) {
            prefilteredColor = texture(skyboxSampler, R, roughness * (MAX_REFLECTION_LOD + 1)).rgb;
        } else {
            prefilteredColor = texture(radianceSampler, R, roughness * MAX_REFLECTION_LOD).rgb;
        }
#ifdef HAVE_IBL_SRGB
        prefilteredColor = sRGB_linear(prefilteredColor); // TODO remove when we have linear HDR env map
#endif
        vec2 envBRDF  = texture(brdfSampler, vec2(max(dot(N, V), 0.0), roughness)).rg;
        vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

        ambient = (kD * diffuse + specular) * ao;
    } else {
        vec3 ambient = vec3(0.03) * albedo * ao;
    }

    vec3 color = ambient + Lo;

#ifdef HAVE_EMISSIVE_SAMPLER
    const float emissiveFactor = 1.0;
    color += (emissiveFactor * emissive);
#endif

    color = tonemapAuto(color);

    FragColor = vec4(color, 1.0);
}
