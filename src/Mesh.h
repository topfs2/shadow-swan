#pragma once
#include <memory>
#include "Image.h"
#include "Geometry.h"
#include "Material.h"

class Mesh
{
public:
    Mesh(GeometryPtr geometry, MaterialPtr material, glm::mat4 model = glm::mat4());
    ~Mesh();

    void draw(ShaderPtr shader, const glm::mat4 &model, GLint unit);

private:
    GeometryPtr m_geometry;
    MaterialPtr m_material;
    glm::mat4 m_model;
};

typedef std::shared_ptr<Mesh> MeshPtr;
