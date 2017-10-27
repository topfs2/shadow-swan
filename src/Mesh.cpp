#include "Mesh.h"

Mesh::Mesh(GeometryPtr geometry, MaterialPtr material) : m_geometry(geometry), m_material(material) { }
Mesh::~Mesh() { }

void Mesh::draw(ShaderPtr shader, const glm::mat4 &view, const glm::mat4 &projection, const glm::mat4 &model, GLint unit) {
    shader->uniform("view", view);
    shader->uniform("projection", projection);
    shader->uniform("model", model);

    m_material->use(shader, unit);
    m_geometry->draw();
}
