#pragma once
#include <memory>
#include "Image.h"
#include "Shader.h"

class Material
{
public:
    Material(ImagePtr albedo, ImagePtr metalRoughnessOcclusion, ImagePtr normal, ImagePtr emissive);
    Material(ImagePtr albedo, ImagePtr metalRoughness, ImagePtr occlusion, ImagePtr normal, ImagePtr emissive);
    Material(ImagePtr albedo, ImagePtr metal, ImagePtr roughness, ImagePtr occlusion, ImagePtr normal, ImagePtr emissive);
    ~Material();

    unsigned int use(ShaderPtr shader, GLint unit);

private:
    void bind(ShaderPtr shader, std::string type, ImagePtr texture, GLint unit);

    ImagePtr m_albedo;
    ImagePtr m_metal;
    ImagePtr m_roughness;
    ImagePtr m_occlusion;
    ImagePtr m_normal;
    ImagePtr m_emissive;
};

typedef std::shared_ptr<Material> MaterialPtr;
