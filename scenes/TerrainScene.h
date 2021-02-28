#pragma once
#include "BaseScene.h"
#include "Texture.h"
#include "Geometry.h"
#include "Shader.h"

class TerrainScene : public BaseScene
{
public:
    TerrainScene();
    virtual ~TerrainScene();

    virtual void OnRender(float t, float dt);

private:
    ShaderPtr m_shader;
    TexturePtr m_texture;
    GeometryPtr m_mesh;
};
