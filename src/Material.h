#pragma once
#include <memory>
#include "Image.h"
#include "Shader.h"

class Material
{
public:
    Material(ImagePtr albedo, ImagePtr metalRoughness, ImagePtr AO, ImagePtr normal, ImagePtr emissive);
    ~Material();

    unsigned int use(ShaderPtr shader, GLint unit);

private:
    void bind(ShaderPtr shader, std::string type, ImagePtr texture, GLint unit);

    ImagePtr m_albedo;
    ImagePtr m_metalRoughness;
    ImagePtr m_AO;
    ImagePtr m_normal;
    ImagePtr m_emissive;
};

typedef std::shared_ptr<Material> MaterialPtr;
