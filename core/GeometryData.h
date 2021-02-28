#pragma once
#include <memory>
#include "Geometry.h"

class GeometryData {  
public:
    GeometryData(unsigned int vertices);
    GeometryData(unsigned int vertices, unsigned int indices);
    GeometryData(const Geometry::varray &vertices);
    GeometryData(const Geometry::varray &vertices, const Geometry::iarray &indices);

    void position(size_t index, float x, float y, float z);
    void position(size_t index, glm::vec3 v);
    glm::vec3 position(size_t index) const;

    void normal(size_t index, float nx, float ny, float nz);
    void normal(size_t index, glm::vec3 v);
    glm::vec3 normal(size_t index) const;

    void uv(size_t index, float u, float v);
    void uv(size_t index, glm::vec2 v);
    glm::vec2 uv(size_t index) const;

    void tangent(size_t index, float x, float y, float z);
    void tangent(size_t index, glm::vec3 v);
    glm::vec3 tangent(size_t index) const;

    void bitangent(size_t index, float x, float y, float z);
    void bitangent(size_t index, glm::vec3 v);
    glm::vec3 bitangent(size_t index) const;
    
    void vertex(size_t index, float x, float y, float z, float nx, float ny, float nz, float u, float v);
    void vertex(size_t index, float x, float y, float z, float u, float v);
    void vertex(size_t index, glm::vec3 position, glm::vec3 normal, glm::vec2 uv, glm::vec3 tangent, glm::vec3 bitangent);
    void vertex(size_t index, glm::vec3 position, glm::vec3 normal, glm::vec2 uv);
    void vertex(size_t index, glm::vec3 position, glm::vec2 uv);
    void vertex(size_t index, Geometry::vertex_t v);
    Geometry::vertex_t vertex(size_t index) const;

    void indices(const Geometry::iarray &indices);
    Geometry::iarray indices() const;

    void triangle(size_t triangle_index, size_t v0, size_t v1, size_t v2);

    void vertices(const Geometry::varray &vertices);
    Geometry::varray vertices() const;

    const Geometry::vertex_t *verticesRaw() const;
    size_t verticesSize() const;

    const GLuint *indicesRaw() const;
    size_t indicesSize() const;

    void computeNormals();
    void computeTangents();

    GeometryPtr build() const { return std::make_shared<Geometry>(m_vertices, m_indices); }

private:
    Geometry::varray m_vertices;
    Geometry::iarray m_indices;
};

typedef std::shared_ptr<GeometryData> GeometryDataPtr;
