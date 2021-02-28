#pragma once

#include <glm/glm.hpp>

class TrackballCamera
{
public:
    TrackballCamera();
    ~TrackballCamera();

    glm::mat4 viewMatrix();
    glm::mat4 viewMatrixSkybox();
    glm::vec3 position();

    void rotate(glm::vec2 offset);
    void zoom(float dDistance);

private:
    glm::vec3 front();

    glm::vec3 m_target;

    float m_theta;
    float m_phi;
    float m_radius;
};
