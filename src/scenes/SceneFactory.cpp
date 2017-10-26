#include "SceneFactory.h"
#include "SampleScene.h"
#include "PBRScene.h"

IScene *SceneFactory::create(std::string name)
{
    if (name == "pbr") {
        return new PBRScene();
    }

    return new SampleScene();
}
