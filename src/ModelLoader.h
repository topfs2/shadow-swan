#pragma once
#include "Geometry.h"
#include <assimp/scene.h>
#include <string>
#include <vector>

class ModelLoader
{
public:
    static std::vector<GeometryPtr> loadGeometries(const std::string &path);

private:
    static GeometryPtr loadGeometry(const aiMesh *mesh);
};
