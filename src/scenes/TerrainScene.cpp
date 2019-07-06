#include "TerrainScene.h"
#include "../GeometryFactory.h"
#include <imgui.h>

TerrainScene::TerrainScene() : BaseScene() {
    m_texture = Texture::create("resources/textures/test-dxt5.dds");
    m_mesh = GeometryFactory::geoclip();
    m_shader = std::make_shared<Shader>("shaders/terrain.vs", "shaders/terrain.fs");
}

TerrainScene::~TerrainScene() { }

void TerrainScene::OnRender(float t, float dt) {
	static bool wireframe = true;
    ImGui::Checkbox("Wireframe", &wireframe);

    if (wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader->use();

    m_texture->bind(0);
    m_shader->uniform("debugTexture", 0);

    m_mesh->draw();
}
