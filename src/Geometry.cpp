#include "Geometry.h"

Geometry::Geometry() : VBO(0), VAO(0), m_count(0) { }

Geometry::Geometry(GeometryDataPtr geometry) : VBO(0), VAO(0), m_count(0) {
    refresh(geometry);
}

Geometry::~Geometry() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Geometry::refresh(GeometryDataPtr geometry) {
    const GeometryData::varray &vertices = geometry->vertices();
    const GeometryData::iarray &indices = geometry->indices();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GeometryData::vertex_t), 
                 &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), 
                 &indices[0], GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryData::vertex_t), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryData::vertex_t), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GeometryData::vertex_t), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // Tangent attribute
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryData::vertex_t), (GLvoid*)(8 * sizeof(GLfloat)));
    glEnableVertexAttribArray(3);

    // Bitangent attribute
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(GeometryData::vertex_t), (GLvoid*)(11 * sizeof(GLfloat)));
    glEnableVertexAttribArray(4);

    m_count = indices.size();

    glBindVertexArray(0); // Unbind VAO
}

void Geometry::draw() {
    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, m_count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Geometry::unbind() {
    glBindVertexArray(0);
}