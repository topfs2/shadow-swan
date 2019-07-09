#include "GeometryFactory.h"
#include "GeometryData.h"

GeometryPtr GeometryFactory::cube(glm::vec3 size) {
    glm::vec3 s2 = size / 2.0f;

    GeometryData geometryData(36);
    int i = 0;

    geometryData.vertex(i++, -s2.x, -s2.y, -s2.z,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f);
    geometryData.vertex(i++,  s2.x, -s2.y, -s2.z,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f);
    geometryData.vertex(i++,  s2.x,  s2.y, -s2.z,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f);
    geometryData.vertex(i++,  s2.x,  s2.y, -s2.z,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f);
    geometryData.vertex(i++, -s2.x,  s2.y, -s2.z,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f);
    geometryData.vertex(i++, -s2.x, -s2.y, -s2.z,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f);

    geometryData.vertex(i++, -s2.x, -s2.y,  s2.z,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f);
    geometryData.vertex(i++,  s2.x, -s2.y,  s2.z,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f);
    geometryData.vertex(i++,  s2.x,  s2.y,  s2.z,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f);
    geometryData.vertex(i++,  s2.x,  s2.y,  s2.z,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f);
    geometryData.vertex(i++, -s2.x,  s2.y,  s2.z,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f);
    geometryData.vertex(i++, -s2.x, -s2.y,  s2.z,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f);

    geometryData.vertex(i++, -s2.x,  s2.y,  s2.z, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f);
    geometryData.vertex(i++, -s2.x,  s2.y, -s2.z, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f);
    geometryData.vertex(i++, -s2.x, -s2.y, -s2.z, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f);
    geometryData.vertex(i++, -s2.x, -s2.y, -s2.z, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f);
    geometryData.vertex(i++, -s2.x, -s2.y,  s2.z, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f);
    geometryData.vertex(i++, -s2.x,  s2.y,  s2.z, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f);

    geometryData.vertex(i++,  s2.x,  s2.y,  s2.z,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f);
    geometryData.vertex(i++,  s2.x,  s2.y, -s2.z,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f);
    geometryData.vertex(i++,  s2.x, -s2.y, -s2.z,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f);
    geometryData.vertex(i++,  s2.x, -s2.y, -s2.z,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f);
    geometryData.vertex(i++,  s2.x, -s2.y,  s2.z,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f);
    geometryData.vertex(i++,  s2.x,  s2.y,  s2.z,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f);

    geometryData.vertex(i++, -s2.x, -s2.y, -s2.z,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f);
    geometryData.vertex(i++,  s2.x, -s2.y, -s2.z,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f);
    geometryData.vertex(i++,  s2.x, -s2.y,  s2.z,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f);
    geometryData.vertex(i++,  s2.x, -s2.y,  s2.z,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f);
    geometryData.vertex(i++, -s2.x, -s2.y,  s2.z,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f);
    geometryData.vertex(i++, -s2.x, -s2.y, -s2.z,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f);

    geometryData.vertex(i++, -s2.x,  s2.y, -s2.z,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f);
    geometryData.vertex(i++,  s2.x,  s2.y, -s2.z,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f);
    geometryData.vertex(i++,  s2.x,  s2.y,  s2.z,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f);
    geometryData.vertex(i++,  s2.x,  s2.y,  s2.z,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f);
    geometryData.vertex(i++, -s2.x,  s2.y,  s2.z,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f);
    geometryData.vertex(i++, -s2.x,  s2.y, -s2.z,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f);

    geometryData.computeTangents();

    return geometryData.build();
}

GeometryPtr GeometryFactory::quad(glm::vec2 size) {
    glm::vec2 s2 = size / 2.0f;

    GeometryData geometryData(4, 6);

    geometryData.vertex(0, -s2.x, -s2.y, 0.0f, 0.0f, 0.0f);
    geometryData.vertex(1,  s2.x, -s2.y, 0.0f, 1.0f, 0.0f);
    geometryData.vertex(2,  s2.x,  s2.y, 0.0f, 1.0f, 1.0f);
    geometryData.vertex(3, -s2.x,  s2.y, 0.0f, 0.0f, 1.0f);

    geometryData.triangle(0, 0, 1, 2);
    geometryData.triangle(1, 0, 2, 3);

    geometryData.computeNormals();
    geometryData.computeTangents();

    return geometryData.build();
}
