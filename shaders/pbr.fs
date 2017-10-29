#version 330 core

#include "tonemap.glsl"

struct Material {
    sampler2D albedoSampler;
    sampler2D metalSampler;
    sampler2D roughnessSampler;
    sampler2D occlusionSampler;
    sampler2D emissiveSampler;
    sampler2D normalSampler;
};

struct Light {
    vec3 position;
    vec3 color;
    mat4 matrix;
    sampler2D shadowMapSampler;
};

uniform samplerCube skyboxSampler;
uniform samplerCube irradianceSampler;
uniform samplerCube radianceSampler;
uniform sampler2D brdfSampler;

uniform bool useIBL;
uniform bool useCheapIBL;
uniform bool useNormalMapping;

uniform Material material;
uniform vec3 viewPos;

uniform int howManyLights;
uniform Light lights[MAX_LIGHTS];

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    mat3 TBN;
    vec3 Normal;
    vec4 FragPosLightSpace[MAX_LIGHTS];
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

float ShadowCalculation(vec4 fragPosLightSpace, float NdL, sampler2D shadowMap)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Check whether current frag pos is in shadow

    float bias = max(0.05 * (1.0 - NdL), 0.005);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    //float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}


void main()
{
    vec3 N = normalize(fs_in.Normal);

    if (useNormalMapping) {
        N = texture(material.normalSampler, fs_in.TexCoords).rgb;
        N = normalize(N * 2.0 - 1.0);
        N = normalize(fs_in.TBN * N);
    }

    vec3 albedo = texture(material.albedoSampler, fs_in.TexCoords).rgb;
    float occlusion = texture(material.occlusionSampler, fs_in.TexCoords).r;
    float metallic = texture(material.metalSampler, fs_in.TexCoords).b;
    float roughness = texture(material.roughnessSampler, fs_in.TexCoords).g;
    vec3 emissive = sRGB_linear(texture(material.emissiveSampler, fs_in.TexCoords).rgb);

    vec3 WorldPos = fs_in.FragPos;

    vec3 V = normalize(viewPos - WorldPos);
    vec3 R = -normalize(reflect(V, N));

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0);

#ifdef MAX_LIGHTS
    for (int i = 0; i < howManyLights; i++) {
        // calculate per-light radiance
        vec3 L = normalize(lights[i].position - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lights[i].position - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lights[i].color * attenuation;

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
        float shadow = ShadowCalculation(fs_in.FragPosLightSpace[i], NdotL, lights[i].shadowMapSampler);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL * (1.0 - shadow);  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
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

        vec3 diffuse    = irradiance * albedo;

        const float MAX_REFLECTION_LOD = 7.0;
        vec3 prefilteredColor = vec3(0);

        if (useCheapIBL) {
            prefilteredColor = texture(skyboxSampler, R, roughness * (MAX_REFLECTION_LOD + 1)).rgb;
        } else {
            prefilteredColor = texture(radianceSampler, R, roughness * MAX_REFLECTION_LOD).rgb;
        }

        vec2 envBRDF  = texture(brdfSampler, vec2(max(dot(N, V), 0.0), roughness)).rg;
        vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

        ambient = (kD * diffuse + specular) * occlusion;
    } else {
        vec3 ambient = vec3(0.03) * albedo * occlusion;
    }

    vec3 color = ambient + Lo + emissive;
    color = tonemapAuto(color);

    FragColor = vec4(color, 1.0);
}
