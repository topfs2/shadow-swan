#include "BaseScene.h"
#include <glm/gtc/matrix_transform.hpp>

BaseScene::BaseScene() : m_camera(),
                         m_projection(glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f))
{
}

BaseScene::~BaseScene()
{

}

void BaseScene::OnResize(unsigned int width, unsigned int height)
{
    m_projection = glm::perspective(45.0f, (float)width/(float)height, 0.1f, 1000.0f);
}

void BaseScene::OnMouseMove(glm::vec2 position, glm::vec2 offset)
{
    m_camera.rotate(offset * glm::vec2(-0.01f));
}
