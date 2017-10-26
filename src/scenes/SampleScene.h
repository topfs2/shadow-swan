#pragma once
#include "BaseScene.h"
#include "../Image.h"
#include "../Geometry.h"
#include "../Shader.h"

class SampleScene : public BaseScene
{
public:
    SampleScene();
    virtual ~SampleScene();

    virtual void OnKey(int key, int scancode, int action, int mode);
    virtual void OnRender(float t, float dt);

private:
    ShaderPtr m_shader;
    ImagePtr m_texture;
    GeometryPtr m_quad;
};
