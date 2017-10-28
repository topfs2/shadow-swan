#include "Material.h"

Material::Material(ImagePtr albedo, ImagePtr metalRoughnessOcclusion, ImagePtr normal, ImagePtr emissive) : m_albedo(albedo), m_metal(metalRoughnessOcclusion), m_roughness(metalRoughnessOcclusion), m_occlusion(metalRoughnessOcclusion), m_normal(normal), m_emissive(emissive) { }
Material::Material(ImagePtr albedo, ImagePtr metalRoughness, ImagePtr occlusion, ImagePtr normal, ImagePtr emissive) : m_albedo(albedo), m_metal(metalRoughness), m_roughness(metalRoughness), m_occlusion(occlusion), m_normal(normal), m_emissive(emissive) { }
Material::Material(ImagePtr albedo, ImagePtr metal, ImagePtr roughness, ImagePtr occlusion, ImagePtr normal, ImagePtr emissive) : m_albedo(albedo), m_metal(metal), m_roughness(roughness), m_occlusion(occlusion), m_normal(normal), m_emissive(emissive) { }

Material::~Material() { }

unsigned int Material::use(ShaderPtr shader, GLint unit) {
    shader->use();

    bind(shader, "albedo", m_albedo, unit++);
    bind(shader, "metal", m_metal, unit++);
    bind(shader, "roughness", m_roughness, unit++);
    bind(shader, "occlusion", m_occlusion, unit++);
    bind(shader, "normal", m_normal, unit++);
    bind(shader, "emissive", m_emissive, unit++);

    return unit;
}

void Material::bind(ShaderPtr shader, std::string type, ImagePtr texture, GLint unit) {
    texture->bind(unit);
    shader->uniform("material." + type + "Sampler", unit);
}
