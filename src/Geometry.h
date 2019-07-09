#pragma once
#include <memory>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glad/glad.h>
#include <vector>

class Geometry
{
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

    Geometry();
    Geometry(const varray &vertices);
    Geometry(const varray &vertices, const iarray &indices);

    ~Geometry();

    void refresh(const varray &vertices);
    void refresh(const varray &vertices, const iarray &indices);

    void draw();
    static void unbind();

private:
    GLuint VAO, VBO, EBO;
    GLuint m_count;
};

typedef std::shared_ptr<Geometry> GeometryPtr;
