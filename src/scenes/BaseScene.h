#pragma once
#include "IScene.h"
#include "../TrackballCamera.h"

class BaseScene : public IScene
{
public:
    BaseScene();
    virtual ~BaseScene();

    virtual void OnResize(unsigned int width, unsigned int height);
    virtual void OnKey(int key, int scancode, int action, int mode);
    virtual void OnMouseMove(glm::vec2 position, glm::vec2 offset);

protected:
    TrackballCamera m_camera;
    glm::mat4 m_projection;
};
