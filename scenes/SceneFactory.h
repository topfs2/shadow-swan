#pragma once
#include <string>
#include "IScene.h"

class SceneFactory
{
public:
    static IScene *create(std::string name);
};
