#include "Material.h"

Material::Material(ImagePtr albedo, ImagePtr metalRoughness, ImagePtr AO, ImagePtr normal, ImagePtr emissive) : m_albedo(albedo), m_metalRoughness(metalRoughness), m_AO(AO), m_normal(normal), m_emissive(emissive) { }

Material::~Material() { }

unsigned int Material::use(ShaderPtr shader, GLint unit) {
    shader->use();

    bind(shader, "albedo", m_albedo, unit++);
    bind(shader, "metalRoughness", m_metalRoughness, unit++);
    bind(shader, "AO", m_AO, unit++);
    bind(shader, "normal", m_normal, unit++);
    bind(shader, "emissive", m_emissive, unit++);

    return unit;
}

void Material::bind(ShaderPtr shader, std::string type, ImagePtr texture, GLint unit) {
    texture->bind(unit);
    shader->uniform("material." + type + "Sampler", unit);
}
