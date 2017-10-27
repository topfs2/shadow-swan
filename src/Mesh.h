#pragma once
#include <memory>
#include "Image.h"
#include "Geometry.h"
#include "Material.h"

class Mesh
{
public:
    Mesh(GeometryPtr geometry, MaterialPtr material);
    ~Mesh();

    void draw(ShaderPtr shader, const glm::mat4 &view, const glm::mat4 &projection, const glm::mat4 &model, GLint unit);

private:
    GeometryPtr m_geometry;
    MaterialPtr m_material;
};

typedef std::shared_ptr<Mesh> MeshPtr;
