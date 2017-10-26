#include "TrackballCamera.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

TrackballCamera::TrackballCamera() : m_target(vec3(0.0f)), m_theta(radians(180.0f)), m_phi(radians(90.0f)), m_radius(3.0f) { }
TrackballCamera::~TrackballCamera() { }

mat4 TrackballCamera::viewMatrix()
{
    vec3 position = this->position();

    vec3 look = normalize(front());
    vec3 worldUp = vec3(0.0f, 1.0f, 0.0f);

    vec3 right = normalize(cross(look, worldUp));
    vec3 up = normalize(cross(right, look));

    return lookAt(position, m_target, up);
}

mat4 TrackballCamera::viewMatrixSkybox()
{
    vec3 position = this->position();

    vec3 look = normalize(front());
    vec3 worldUp = vec3(0.0f, 1.0f, 0.0f);

    vec3 right = normalize(cross(look, worldUp));
    vec3 up = normalize(cross(right, look));

    return lookAt(vec3(0.0f), look, up);
}

vec3 TrackballCamera::position()
{
    return m_target - front();
}

void TrackballCamera::rotate(vec2 offset)
{
    m_theta += offset.x;
    m_phi += offset.y;

    m_phi = clamp(m_phi, radians(1.0f), radians(179.0f));
}

void TrackballCamera::zoom(float dDistance)
{
    m_radius += dDistance;
}

#include <cstdio>

vec3 TrackballCamera::front()
{
    float x = m_radius * sinf(m_phi) * sinf(m_theta);
    float y = m_radius * cosf(m_phi);
    float z = m_radius * sinf(m_phi) * cosf(m_theta);

    return vec3(x, y, z);
}
