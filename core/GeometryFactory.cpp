#include "GeometryFactory.h"
#include "GeometryData.h"
#include <algorithm>

// Taken from https://github.com/morgan3d/misc/blob/master/terrain/default.Terrain.Any
const int numMeshLODLevels = 7;
const int meshBaseLODExtentHeightfieldTexels = 128;

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

void addTriangle(Geometry::varray &vertices, const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c) {
    vertices.emplace_back(a);
    vertices.emplace_back(b);
    vertices.emplace_back(c);
}

// https://github.com/morgan3d/misc/blob/master/terrain/Terrain.cpp#L139
GeometryPtr GeometryFactory::geoclip() {
    Geometry::varray vertices;

    // Store LOD in the Y coordinate, store XZ on the texel grid resolution
    for (int level = 0; level < numMeshLODLevels; ++level) {
        const int step = (1 << level);
        const int prevStep = std::max(0, (1 << (level - 1)));
        const int halfStep = prevStep;

        const int g = meshBaseLODExtentHeightfieldTexels / 2;
        const float L = float(level);

        // Move up one grid level; used when stitching
        const glm::vec3 nextLevel(0, 1, 0);

        // Pad by one element to hide the gap to the next level
        const int pad = 1;
        const int radius = step * (g + pad);
        for (int z = -radius; z < radius; z += step) {
            for (int x = -radius; x < radius; x += step) {
                if (std::max(std::abs(x + halfStep), std::abs(z + halfStep)) >= g * prevStep) {
                    // Cleared the cutout from the previous level. Tessellate the
                    // square.

                    //   A-----B-----C
                    //   | \   |   / |
                    //   |   \ | /   |
                    //   D-----E-----F
                    //   |   / | \   |
                    //   | /   |   \ |
                    //   G-----H-----I

                    const glm::vec3 A(float(x), L, float(z));
                    const glm::vec3 C(float(x + step), L, A.z);
                    const glm::vec3 G(A.x, L, float(z + step));
                    const glm::vec3 I(C.x, L, G.z);

                    const glm::vec3 B = (A + C) * 0.5f;
                    const glm::vec3 D = (A + G) * 0.5f;
                    const glm::vec3 F = (C + I) * 0.5f;
                    const glm::vec3 H = (G + I) * 0.5f;

                    const glm::vec3 E = (A + I) * 0.5f;

                    if (x == -radius) {
                        //   A-----B-----C
                        //   | \   |   / |
                        //   |   \ | /   |
                        //   |     E-----F
                        //   |   / | \   |
                        //   | /   |   \ |
                        //   G-----H-----I
                        addTriangle(vertices, E, A, G);
                    } else {
                        addTriangle(vertices, E, A, D);
                        addTriangle(vertices, E, D, G);
                    }

                    if (z == radius - 1) {
                        addTriangle(vertices, E, G, I);
                    } else {
                        addTriangle(vertices, E, G, H);
                        addTriangle(vertices, E, H, I);
                    }

                    if (x == radius - 1) {
                        addTriangle(vertices, E, I, C);
                    } else {
                        addTriangle(vertices, E, I, F);
                        addTriangle(vertices, E, F, C);
                    }

                    if (z == -radius) {
                        addTriangle(vertices, E, C, A);
                    } else {
                        addTriangle(vertices, E, C, B);
                        addTriangle(vertices, E, B, A);
                    }
                } // if
            } // x
        } // z
    } // level

    return std::make_shared<Geometry>(vertices);
}
