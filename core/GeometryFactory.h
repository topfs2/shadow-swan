#pragma once
#include <memory>
#include "Geometry.h"

class GeometryFactory
{
public:
    static GeometryPtr cube(glm::vec3 size);
    static GeometryPtr quad(glm::vec2 size);
    static GeometryPtr geoclip();
};
