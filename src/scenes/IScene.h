#pragma once

#include <glm/vec2.hpp>

class IScene
{
public:
  virtual ~IScene() { };

  virtual void OnResize(unsigned int width, unsigned int height) = 0;

  virtual void OnKey(int key, int scancode, int action, int mode) = 0;
  virtual void OnMouseMove(glm::vec2 position, glm::vec2 offset) = 0;

  virtual void OnRender(float t, float dt) = 0;
};
