#include "PBRScene.h"
#include "../GeometryFactory.h"
#include "../ModelLoader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>

PBRScene::PBRScene() : BaseScene()
{
    m_tonemap = 5;
    m_bloom = true;
    m_useIBL = true;
    m_lights = 3;
    m_whichIBL = 0;
    m_useCheapIBL = false;

    m_pbrShader = ShaderPtr(new Shader("shaders/pbr.vs", "shaders/pbr.fs", { "MAX_LIGHTS 3", "HDR_TONEMAP", "HAVE_ALBEDO_SRGB", "HAVE_NORMAL_SAMPLER", "HAVE_METAL_ROUGHNESS_SAMPLER", "HAVE_EMISSIVE_SAMPLER", "HAVE_EMISSIVE_SRGB" }));
    m_brightShader = ShaderPtr(new Shader("shaders/bright.vs", "shaders/bright.fs"));
    m_blurShader = ShaderPtr(new Shader("shaders/blur.vs", "shaders/blur.fs"));
    m_compositeShader = ShaderPtr(new Shader("shaders/composite.vs", "shaders/composite.fs", { "GAMMA_CORRECT" }));

    m_skyboxShader = ShaderPtr(new Shader("shaders/skybox.vs", "shaders/skybox.fs", { "HDR_TONEMAP" }));
    m_lampShader = ShaderPtr(new Shader("shaders/lamp.vs", "shaders/lamp.fs", { "HDR_TONEMAP" }));

    m_ibls.push_back(loadIBL("milkyway"));
    m_ibls.push_back(loadIBL("outside"));
    m_ibls.push_back(loadIBL("arches"));

    m_brdf = ImagePtr(new Image("resources/textures/brdfLUT.png"));

    m_fsQuad = GeometryFactory::quad(glm::vec2(2.0f));
    m_geometry = ModelLoader::loadGeometries("resources/gltf2/DamagedHelmet/DamagedHelmet.gltf")[0];
    m_cube = GeometryFactory::cube(glm::vec3(1.0f));

    m_albedo = ImagePtr(new Image("resources/gltf2/DamagedHelmet/Default_albedo.jpg"));
    m_AO = ImagePtr(new Image("resources/gltf2/DamagedHelmet/Default_AO.jpg"));
    m_emissive = ImagePtr(new Image("resources/gltf2/DamagedHelmet/Default_emissive.jpg"));
    m_metalRoughness = ImagePtr(new Image("resources/gltf2/DamagedHelmet/Default_metalRoughness.jpg"));
    m_normal = ImagePtr(new Image("resources/gltf2/DamagedHelmet/Default_normal.jpg"));
}

PBRScene::~PBRScene()
{

}

void PBRScene::OnResize(unsigned int width, unsigned int height)
{
    BaseScene::OnResize(width, height);

    m_gColorOutput = ImagePtr(new Image(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, NULL, false));
    m_colorFramebuffer = FrameBufferPtr(new FrameBuffer(m_gColorOutput, width, height));

    m_gPingOutput = ImagePtr(new Image(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, NULL, false));
    m_pingFramebuffer = FrameBufferPtr(new FrameBuffer(m_gPingOutput, width, height));

    m_gPongOutput = ImagePtr(new Image(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, NULL, false));
    m_pongFramebuffer = FrameBufferPtr(new FrameBuffer(m_gPongOutput, width, height));
}

void PBRScene::OnKey(int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS && key >= GLFW_KEY_1 && key <= GLFW_KEY_6) {
        m_tonemap = key - GLFW_KEY_1;
        printf("tonemap %i\n", m_tonemap);
    } else if (action == GLFW_PRESS && key == GLFW_KEY_I) {
        m_useIBL = !m_useIBL;
        printf("useIBL %s\n", m_useIBL ? "true" : "false");
    } else if (action == GLFW_PRESS && key == GLFW_KEY_C) {
        m_useCheapIBL = !m_useCheapIBL;
        printf("useCheapIBL %s\n", m_useCheapIBL ? "true" : "false");
    } else if (action == GLFW_PRESS && key == GLFW_KEY_B) {
        m_bloom = !m_bloom;
        printf("bloom %s\n", m_bloom ? "true" : "false");
    } else if (action == GLFW_PRESS && key == GLFW_KEY_L) {
        m_lights = (m_lights + 1) % 4;
        printf("lights %i\n", m_lights);
    } else if (action == GLFW_PRESS && key == GLFW_KEY_P) {
        m_whichIBL = (m_whichIBL + 1) % m_ibls.size();
        printf("whichIBL %i\n", m_whichIBL);
    }
}

void PBRScene::OnRender(float t, float dt)
{
    m_colorFramebuffer->bind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    IBL ibl = m_ibls[m_whichIBL];

    RenderSkybox(ibl.skybox);
    glEnable(GL_DEPTH_TEST);

    glm::vec3 lightPositions[3] = {
        glm::vec3( 0.0f, 5.0f, 0.0f),
        glm::vec3(-3.0f, 5.0f, 0.0f),
        glm::vec3( 3.0f, 5.0f, 0.0f)
    };

    glm::vec3 lightColors[3] = {
        glm::vec3(255.0f),
        glm::vec3(0.0f, 255.0f, 128.0f),
        glm::vec3(255.0f, 128.0f, 0.0f)
    };

    m_pbrShader->use();

    m_pbrShader->uniform("view", m_camera.viewMatrix());
    m_pbrShader->uniform("projection", m_projection);
    m_pbrShader->uniform("viewPos", m_camera.position());

    m_pbrShader->uniform("lights", m_lights);

    for (int i = 0; i < 3; i++) {
        m_pbrShader->uniform("lightPositions[" + std::to_string(i) + "]", lightPositions[i]);
        m_pbrShader->uniform("lightColors[" + std::to_string(i) + "]", lightColors[i]);
    }

    ibl.irradiance->bind(5);
    ibl.radiance->bind(6);
    m_brdf->bind(7);

    m_pbrShader->uniform("irradianceSampler", 5);
    m_pbrShader->uniform("radianceSampler", 6);
    m_pbrShader->uniform("brdfSampler", 7);

    ibl.skybox->bind(8);
    m_pbrShader->uniform("skyboxSampler", 8);


    m_pbrShader->uniform("useIBL", m_useIBL);
    m_pbrShader->uniform("useCheapIBL", m_useCheapIBL);
    m_pbrShader->uniform("tonemap", m_tonemap);

    {
        glm::mat4 model;

        model = glm::rotate(model, 3.14f, glm::vec3(0, 1, 0));
        model = glm::rotate(model, 3.14f / 2.0f, glm::vec3(1, 0, 0));
        //model = glm::rotate(model, t * 0.2f, glm::vec3(1.0f, 1.0f, 1.0f));

        m_pbrShader->uniform("model", model);

        m_albedo->bind(0);
        m_AO->bind(1);
        m_emissive->bind(2);
        m_metalRoughness->bind(3);
        m_normal->bind(4);

        m_pbrShader->uniform("material.albedoSampler", 0);
        m_pbrShader->uniform("material.AOSampler", 1);
        m_pbrShader->uniform("material.emissiveSampler", 2);
        m_pbrShader->uniform("material.metalRoughnessSampler", 3);
        m_pbrShader->uniform("material.normalSampler", 4);

        m_geometry->draw();
    }

    for (int i = 0; i < m_lights; i++) {
        RenderLamp(lightPositions[i], lightColors[i]);
    }

    if (true) {
        m_pongFramebuffer->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_brightShader->use();

        m_gColorOutput->bind(0);
        m_brightShader->uniform("inputSampler", 0);

        m_fsQuad->draw();
    }

    for (int i = 0; i < 20; i++) {
        m_pingFramebuffer->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_blurShader->use();

        m_gPongOutput->bind(0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        m_blurShader->uniform("horizontal", true);
        m_blurShader->uniform("inputSampler", 0);

        m_fsQuad->draw();

        m_pongFramebuffer->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_gPingOutput->bind(0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        m_blurShader->uniform("horizontal", false);

        m_fsQuad->draw();
    }

    FrameBuffer::unbind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_compositeShader->use();

    m_gColorOutput->bind(0);
    m_gPongOutput->bind(1);

    m_compositeShader->uniform("colorInput", 0);
    m_compositeShader->uniform("brightInput", 1);
    m_compositeShader->uniform("bloom", m_bloom);

    m_fsQuad->draw();
}

void PBRScene::RenderSkybox(CubemapPtr skybox)
{
    glDepthMask(GL_FALSE);

    m_skyboxShader->use();

    m_skyboxShader->uniform("view", m_camera.viewMatrixSkybox());
    m_skyboxShader->uniform("projection", m_projection);

    m_skyboxShader->uniform("tonemap", m_tonemap);

    skybox->bind(0);
    m_skyboxShader->uniform("skybox", 0);

    m_cube->draw();

    glDepthMask(GL_TRUE);
}

void PBRScene::RenderLamp(glm::vec3 position, glm::vec3 color)
{
    m_lampShader->use();

    glm::mat4 model;
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(0.1f));

    m_lampShader->uniform("model", model);
    m_lampShader->uniform("view", m_camera.viewMatrix());
    m_lampShader->uniform("projection", m_projection);

    m_lampShader->uniform("lightColor", color);
    m_lampShader->uniform("tonemap", m_tonemap);

    m_cube->draw();
}

PBRScene::IBL PBRScene::loadIBL(std::string name)
{
    std::string prefix = "resources/ibl/" + name + "/";

    IBL ibl;

    ibl.skybox = CubemapPtr(new Cubemap(prefix + "skybox_", ".hdr", true));
    ibl.radiance = CubemapPtr(new Cubemap(prefix + "radiance_", ".hdr", true));
    ibl.irradiance = CubemapPtr(new Cubemap(prefix + "irradiance_", ".hdr", false));

    return ibl;
}
