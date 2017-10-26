#pragma once
#include "IScene.h"
#include "../TrackballCamera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class BaseScene : public IScene
{
public:
    BaseScene();
    virtual ~BaseScene();

    virtual void OnResize(unsigned int width, unsigned int height);
    virtual void OnMouseMove(glm::vec2 position, glm::vec2 offset);

protected:
    TrackballCamera m_camera;
    glm::mat4 m_projection;
};
