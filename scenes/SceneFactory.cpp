#include "SceneFactory.h"
#include "SampleScene.h"
#include "PBRScene.h"
#include "TerrainScene.h"

IScene *SceneFactory::create(std::string name)
{
    if (name == "pbr") {
        return new PBRScene();
    } else if (name == "terrain") {
        return new TerrainScene();
    }

    return new SampleScene();
}
