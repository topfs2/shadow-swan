#pragma once
#include <memory>
#include "GeometryData.h"

class Geometry
{
public:
    Geometry();
    Geometry(GeometryDataPtr geometry);

    ~Geometry();

    void refresh(GeometryDataPtr geometry);

    void draw();
    static void unbind();

private:
    GLuint VAO, VBO, EBO;
    GLuint m_count;
};

typedef std::shared_ptr<Geometry> GeometryPtr;
