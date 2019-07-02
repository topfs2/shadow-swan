#pragma once
#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glad/glad.h>
#include <vector>

class GeometryData {  
public:
    struct vertex_t
    {
        vertex_t() : position(0), normal(0), uv(0), tangent(0), bitangent(0) { }
        vertex_t(glm::vec3 _p, glm::vec3 _n, glm::vec2 _uv) : position(_p), normal(_n), uv(_uv), tangent(0), bitangent(0) { }
        vertex_t(glm::vec3 _p, glm::vec3 _n, glm::vec2 _uv, glm::vec3 _t, glm::vec3 _b) : position(_p), normal(_n), uv(_uv), tangent(_t), bitangent(_b) { }

        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };

    typedef std::vector<vertex_t> varray;
    typedef std::vector<GLuint> iarray;

    GeometryData(unsigned int vertices);
    GeometryData(unsigned int vertices, unsigned int indices);
    GeometryData(const varray &vertices);
    GeometryData(const varray &vertices, const iarray &indices);

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
    void vertex(size_t index, vertex_t v);
    vertex_t vertex(size_t index) const;

    void indices(const iarray &indices);
    iarray indices() const;

    void triangle(size_t triangle_index, size_t v0, size_t v1, size_t v2);

    void vertices(const varray &vertices);
    varray vertices() const;

    const vertex_t *verticesRaw() const;
    size_t verticesSize() const;

    const GLuint *indicesRaw() const;
    size_t indicesSize() const;

    void computeNormals();
    void computeTangents();

private:
    varray m_vertices;
    iarray m_indices;
};

typedef std::shared_ptr<GeometryData> GeometryDataPtr;
