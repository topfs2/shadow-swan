#include "GeometryData.h"
#include <glm/gtc/matrix_transform.hpp>

GeometryData::GeometryData(unsigned int vertices) : m_vertices(vertices) {
    for(int i = 0; i < vertices; i++) {
        m_indices.push_back(i);
    }
}

GeometryData::GeometryData(unsigned int vertices, unsigned int indices) : m_vertices(vertices), m_indices(indices) { }

GeometryData::GeometryData(const GeometryData::varray &vertices) : m_vertices(vertices) {
    for(int i = 0; i < vertices.size(); i++) {
        m_indices.push_back(i);
    }
}

GeometryData::GeometryData(const GeometryData::varray &vertices, const GeometryData::iarray &indices) : m_vertices(vertices), m_indices(indices) { }

void GeometryData::position(size_t index, float x, float y, float z)
{
    position(index, glm::vec3(x, y, z));
}

void GeometryData::position(size_t index, glm::vec3 v)
{
    m_vertices[index].position = v;
}

glm::vec3 GeometryData::position(size_t index) const
{
    return m_vertices[index].position;
}

void GeometryData::normal(size_t index, float nx, float ny, float nz)
{
    normal(index, glm::vec3(nx, ny, nz));
}

void GeometryData::normal(size_t index, glm::vec3 v)
{
    m_vertices[index].normal = v;
}

glm::vec3 GeometryData::normal(size_t index) const
{
    return m_vertices[index].normal;
}

void GeometryData::uv(size_t index, float u, float v)
{
    uv(index, glm::vec2(u, v));
}

void GeometryData::uv(size_t index, glm::vec2 v)
{
    m_vertices[index].uv = v;
}

glm::vec2 GeometryData::uv(size_t index) const
{
    return m_vertices[index].uv;
}

void GeometryData::tangent(size_t index, float x, float y, float z)
{
    tangent(index, glm::vec3(x, y, z));
}

void GeometryData::tangent(size_t index, glm::vec3 v)
{
    m_vertices[index].tangent = v;
}

glm::vec3 GeometryData::tangent(size_t index) const
{
    return m_vertices[index].tangent;
}

void GeometryData::bitangent(size_t index, float x, float y, float z)
{
    bitangent(index, glm::vec3(x, y, z));
}

void GeometryData::bitangent(size_t index, glm::vec3 v)
{
    m_vertices[index].bitangent = v;
}

glm::vec3 GeometryData::bitangent(size_t index) const
{
    return m_vertices[index].bitangent;
}

void GeometryData::vertex(size_t index, float x, float y, float z, float nx, float ny, float nz, float u, float v)
{
    vertex(index, glm::vec3(x, y, z), glm::vec3(nx, ny, nz), glm::vec2(u, v));
}

void GeometryData::vertex(size_t index, float x, float y, float z, float u, float v)
{
    vertex(index, glm::vec3(x, y, z), glm::vec2(u, v));
}

void GeometryData::vertex(size_t index, glm::vec3 position, glm::vec3 normal, glm::vec2 uv, glm::vec3 tangent, glm::vec3 bitangent)
{
    vertex(index, vertex_t(position, normal, uv, tangent, bitangent));
}

void GeometryData::vertex(size_t index, glm::vec3 position, glm::vec3 normal, glm::vec2 uv)
{
    vertex(index, vertex_t(position, normal, uv));
}

void GeometryData::vertex(size_t index, glm::vec3 position, glm::vec2 uv)
{
    m_vertices[index].position = position;
    m_vertices[index].uv = uv;
}

void GeometryData::vertex(size_t index, vertex_t v)
{
    m_vertices[index] = v;
}

GeometryData::vertex_t GeometryData::vertex(size_t index) const
{
    return m_vertices[index];
}

void GeometryData::indices(const GeometryData::iarray &indices)
{
    m_indices = indices;
}

GeometryData::iarray GeometryData::indices() const
{
    return m_indices;
}

void GeometryData::triangle(size_t triangle_index, size_t v0, size_t v1, size_t v2)
{
    size_t i = triangle_index * 3;
    m_indices[i] = v0;
    m_indices[i + 1] = v1;
    m_indices[i + 2] = v2;
}

void GeometryData::vertices(const GeometryData::varray &vertices)
{
    m_vertices = vertices;
}

GeometryData::varray GeometryData::vertices() const
{
    return m_vertices;
}

const GeometryData::vertex_t *GeometryData::verticesRaw() const
{
    return &m_vertices.front();
}

size_t GeometryData::verticesSize() const
{
    return m_vertices.size();
}

const GLuint *GeometryData::indicesRaw() const
{
    return &m_indices.front();
}

size_t GeometryData::indicesSize() const
{
    return m_indices.size();
}

void GeometryData::computeNormals()
{
    for (varray::iterator itr = m_vertices.begin(); itr != m_vertices.end(); itr++) {
        itr->normal = glm::vec3(0.0f);
    }

    for (unsigned int i = 0; i < m_indices.size(); i += 3) {
        uint32_t i0 = m_indices[i];
        uint32_t i1 = m_indices[i + 1];
        uint32_t i2 = m_indices[i + 2];

        vertex_t &a = m_vertices[i0];
        vertex_t &b = m_vertices[i1];
        vertex_t &c = m_vertices[i2];

        glm::vec3 n = glm::normalize(glm::cross(b.position - c.position, b.position - a.position));

        a.normal += n;
        b.normal += n;
        c.normal += n;
    }

    for (varray::iterator itr = m_vertices.begin(); itr != m_vertices.end(); itr++) {
        itr->normal = glm::normalize(itr->normal);
    }
}

#include <iostream>
#include <glm/gtx/string_cast.hpp>

void GeometryData::computeTangents()
{
    for (varray::iterator itr = m_vertices.begin(); itr != m_vertices.end(); itr++) {
        itr->tangent = glm::vec3(0.0f);
    }

    for (unsigned int i = 0; i < m_indices.size(); i += 3) {
        uint32_t i0 = m_indices[i];
        uint32_t i1 = m_indices[i + 1];
        uint32_t i2 = m_indices[i + 2];

        vertex_t &a = m_vertices[i0];
        vertex_t &b = m_vertices[i1];
        vertex_t &c = m_vertices[i2];

        glm::vec3 edge1 = b.position - a.position;
        glm::vec3 edge2 = c.position - a.position;
        glm::vec2 deltaUV1 = b.uv - a.uv;
        glm::vec2 deltaUV2 = c.uv - a.uv;

        GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent(
            f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
            f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
            f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
        );
        a.tangent = b.tangent = c.tangent = glm::normalize(tangent);

        glm::vec3 bitangent(
            f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x),
            f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y),
            f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z)
        );
        a.bitangent = b.bitangent = c.bitangent = glm::normalize(bitangent);
    }

    /*
    for (varray::iterator itr = m_vertices.begin(); itr != m_vertices.end(); itr++) {
        std::cout << "normal    " << glm::to_string(itr->normal) << std::endl;
        std::cout << "tangent   " << glm::to_string(itr->tangent) << std::endl;
        std::cout << "bitangent " << glm::to_string(itr->bitangent) << std::endl;
    }
    */
}
