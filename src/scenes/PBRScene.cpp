#include "PBRScene.h"
#include "../GeometryFactory.h"
#include "../ModelLoader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>
#include <string>
#include <random>
#include <iostream>

PBRScene::PBRScene() : BaseScene()
{
    m_tonemap = 5;
    m_bloom = true;
    m_useIBL = true;
    m_howManyLights = 3;
    m_whichIBL = 0;
    m_useCheapIBL = false;
    m_whichMesh = 0;
    m_useNormalMapping = true;
    m_useSSAO = true;

    m_lights.push_back(Light(
        glm::vec3(0.0f, 1.5f, 0.0f),
        glm::vec3(30.0f)
    ));

    m_lights.push_back(Light(
        glm::vec3(-2.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 30.0f, 0.0f)
    ));

    m_lights.push_back(Light(
        glm::vec3(2.0f, 1.0f, 0.0f),
        glm::vec3(30.0f, 00.0f, 0.0f)
    ));


    m_geometryShader = ShaderPtr(new Shader("shaders/geometry.vs", "shaders/geometry.fs"));
    m_ssaoShader = ShaderPtr(new Shader("shaders/ssao.vs", "shaders/ssao.fs"));
    m_ssaoBlurShader = ShaderPtr(new Shader("shaders/ssaoBlur.vs", "shaders/ssaoBlur.fs"));
    m_pbrShader = ShaderPtr(new Shader("shaders/pbr.vs", "shaders/pbr.fs", { "MAX_LIGHTS 3", "HDR_TONEMAP" }));
    m_brightShader = ShaderPtr(new Shader("shaders/bright.vs", "shaders/bright.fs"));
    m_blurShader = ShaderPtr(new Shader("shaders/blur.vs", "shaders/blur.fs"));
    m_compositeShader = ShaderPtr(new Shader("shaders/composite.vs", "shaders/composite.fs", { "GAMMA_CORRECT" }));

    m_skyboxShader = ShaderPtr(new Shader("shaders/skybox.vs", "shaders/skybox.fs", { "HDR_TONEMAP" }));
    m_lightShader = ShaderPtr(new Shader("shaders/light.vs", "shaders/light.fs", { "HDR_TONEMAP" }));

    m_ibls.push_back(loadIBL("milkyway"));
    m_ibls.push_back(loadIBL("outside"));
    m_ibls.push_back(loadIBL("arches"));

    m_brdf = ImagePtr(new Image("resources/textures/brdfLUT.png"));

    m_fsQuad = GeometryFactory::quad(glm::vec2(2.0f));
    m_cube = GeometryFactory::cube(glm::vec3(1.0f));

    ImagePtr black = ImagePtr(new Image("resources/textures/black.jpg"));

    m_meshes.push_back(MeshPtr(new Mesh(
        ModelLoader::loadGeometries("resources/gltf2/DamagedHelmet/DamagedHelmet.gltf")[0],
        MaterialPtr(new Material(
            ImagePtr(new Image("resources/gltf2/DamagedHelmet/Default_albedo.jpg", true)),
            ImagePtr(new Image("resources/gltf2/DamagedHelmet/Default_metalRoughness.jpg", true)),
            ImagePtr(new Image("resources/gltf2/DamagedHelmet/Default_AO.jpg", true)),
            ImagePtr(new Image("resources/gltf2/DamagedHelmet/Default_normal.jpg", true)),
            ImagePtr(new Image("resources/gltf2/DamagedHelmet/Default_emissive.jpg", true))
        )),
        glm::rotate(glm::mat4(1.0f), 3.14f / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f))
    )));

    m_meshes.push_back(MeshPtr(new Mesh(
        ModelLoader::loadGeometries("resources/gltf2/Corset/Corset.gltf")[0],
        MaterialPtr(new Material(
            ImagePtr(new Image("resources/gltf2/Corset/Corset_baseColor.png", true)),
            ImagePtr(new Image("resources/gltf2/Corset/Corset_occlusionRoughnessMetallic.png", true)),
            ImagePtr(new Image("resources/gltf2/Corset/Corset_normal.png", true)),
            black
        )),
        glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.9f, 0.0f)), glm::vec3(30.0f))
    )));

    m_meshes.push_back(MeshPtr(new Mesh(
        ModelLoader::loadGeometries("resources/gltf2/BoomBox/BoomBox.gltf")[0],
        MaterialPtr(new Material(
            ImagePtr(new Image("resources/gltf2/BoomBox/BoomBox_baseColor.png", true)),
            ImagePtr(new Image("resources/gltf2/BoomBox/BoomBox_occlusionRoughnessMetallic.png", true)),
            ImagePtr(new Image("resources/gltf2/BoomBox/BoomBox_normal.png", true)),
            ImagePtr(new Image("resources/gltf2/BoomBox/BoomBox_emissive.png", true))
        )),
        glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.25f, 0.0f)), glm::vec3(70.0f))
    )));

    m_meshes.push_back(MeshPtr(new Mesh(
        ModelLoader::loadGeometries("resources/gltf2/Telephone/Telephone.gltf")[0],
        MaterialPtr(new Material(
            ImagePtr(new Image("resources/gltf2/Telephone/Telephone_baseColor.png", true)),
            ImagePtr(new Image("resources/gltf2/Telephone/Telephone_occlusionRoughnessMetallic.png", true)),
            ImagePtr(new Image("resources/gltf2/Telephone/Telephone_normal.png", true)),
            black
        )),
        glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.9f, 0.0f)), glm::vec3(10.0f))
    )));

    m_ground = MeshPtr(new Mesh(
        GeometryFactory::cube(glm::vec3(8.0f, 0.5f, 8.0f)),
        MaterialPtr(new Material(
            ImagePtr(new Image("resources/textures/granite/graniterockface1_Base_Color.png")),
            black,
            ImagePtr(new Image("resources/textures/granite/graniterockface1_Roughness.png")),
            ImagePtr(new Image("resources/textures/granite/graniterockface1_Ambient_Occlusion.png")),
            ImagePtr(new Image("resources/textures/granite/graniterockface1_Normal.png")),
            black
        )),
        glm::translate(glm::mat4(), glm::vec3(0.0f, -1.2f, 0.0f))
    ));

    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
    std::default_random_engine generator;
    for (GLuint i = 0; i < 64; ++i)
    {
        glm::vec3 sample(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator)
        );
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        float scale = float(i) / 64.0;

        scale = glm::lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        m_ssaoKernel.push_back(sample);
    }

    std::vector<glm::vec3> ssaoNoise;
    for (GLuint i = 0; i < 16; i++)
    {
        glm::vec3 noise(
            randomFloats(generator) * 2.0 - 1.0,
            randomFloats(generator) * 2.0 - 1.0,
            0.0f);
        ssaoNoise.push_back(noise);
    }

    m_noiseTexture = ImagePtr(new Image(4, 4, GL_RGB16F, GL_RGB, GL_FLOAT, &ssaoNoise[0], false, GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT));
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

    m_gPosition = ImagePtr(new Image(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, NULL, false));
    m_gNormal = ImagePtr(new Image(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, NULL, false));
    m_geometryFramebuffer = FrameBufferPtr(new FrameBuffer({ m_gPosition, m_gNormal }, width, height));

    m_ssaoInput = ImagePtr(new Image(width, height, GL_RED, GL_RGB, GL_FLOAT, NULL, false));
    m_ssaoFrameBuffer = FrameBufferPtr(new FrameBuffer(m_ssaoInput, width, height));

    m_ssao = ImagePtr(new Image(width, height, GL_RED, GL_RGB, GL_FLOAT, NULL, false));
    m_ssaoBlurFrameBuffer = FrameBufferPtr(new FrameBuffer(m_ssao, width, height));

    glm::vec2 size((float)width, (float)height);
    m_noiseScale = size / 4.0f;
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
        m_howManyLights = (m_howManyLights + 1) % (m_lights.size() + 1);
        printf("howManyLights %i\n", m_howManyLights);
    } else if (action == GLFW_PRESS && key == GLFW_KEY_P) {
        m_whichIBL = (m_whichIBL + 1) % m_ibls.size();
        printf("whichIBL %i\n", m_whichIBL);
    } else if (action == GLFW_PRESS && key == GLFW_KEY_M) {
        m_whichMesh = (m_whichMesh + 1) % m_meshes.size();
        printf("whichMesh %i\n", m_whichMesh);
    } else if (action == GLFW_PRESS && key == GLFW_KEY_N) {
        m_useNormalMapping = !m_useNormalMapping;
        printf("useNormalMapping %s\n", m_useNormalMapping ? "true" : "false");
    } else if (action == GLFW_PRESS && key == GLFW_KEY_S) {
        m_useSSAO = !m_useSSAO;
        printf("useSSAO %s\n", m_useSSAO ? "true" : "false");
    }
}

void PBRScene::OnRender(float t, float dt)
{
    if (m_useSSAO) {
        m_geometryFramebuffer->bind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_geometryShader->use();
        m_geometryShader->uniform("useNormalMapping", m_useNormalMapping);

        m_meshes[m_whichMesh]->draw(m_geometryShader, m_camera.viewMatrix(), m_projection, glm::mat4(), 4);
        m_ground->draw(m_geometryShader, m_camera.viewMatrix(), m_projection, glm::mat4(), 4);


        m_ssaoFrameBuffer->bind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_ssaoShader->use();

        m_ssaoShader->uniform("gPosition", 0);
        m_gPosition->bind(0);

        m_ssaoShader->uniform("gNormal", 1);
        m_gNormal->bind(1);

        m_ssaoShader->uniform("texNoise", 2);
        m_noiseTexture->bind(2);

        m_ssaoShader->uniform("samples", m_ssaoKernel);
        m_ssaoShader->uniform("noiseScale", m_noiseScale);
        m_ssaoShader->uniform("projection", m_projection);

        m_fsQuad->draw();

        m_ssaoBlurFrameBuffer->bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_ssaoBlurShader->use();
        m_ssaoBlurShader->uniform("ssaoInput", 0);
        m_ssaoInput->bind(0);

        m_fsQuad->draw();
    }

    m_colorFramebuffer->bind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    IBL ibl = m_ibls[m_whichIBL];

    RenderSkybox(ibl.skybox);
    glEnable(GL_DEPTH_TEST);

    m_pbrShader->use();

    m_pbrShader->uniform("viewPos", m_camera.position());

    m_pbrShader->uniform("howManyLights", m_howManyLights);

    for (int i = 0; i < m_lights.size(); i++) {
        Light light = m_lights[i];
        std::string prefix = "lights[" + std::to_string(i) + "].";
        m_pbrShader->uniform(prefix + "position", light.position);
        m_pbrShader->uniform(prefix + "color", light.color);
    }

    ibl.skybox->bind(0);
    ibl.irradiance->bind(1);
    ibl.radiance->bind(2);
    m_brdf->bind(3);

    m_pbrShader->uniform("skyboxSampler", 0);
    m_pbrShader->uniform("irradianceSampler", 1);
    m_pbrShader->uniform("radianceSampler", 2);
    m_pbrShader->uniform("brdfSampler", 3);

    m_pbrShader->uniform("useIBL", m_useIBL);
    m_pbrShader->uniform("useCheapIBL", m_useCheapIBL);
    m_pbrShader->uniform("tonemap", m_tonemap);
    m_pbrShader->uniform("useNormalMapping", m_useNormalMapping);

    m_meshes[m_whichMesh]->draw(m_pbrShader, m_camera.viewMatrix(), m_projection, glm::mat4(), 4);
    m_ground->draw(m_pbrShader, m_camera.viewMatrix(), m_projection, glm::mat4(), 4);

    RenderLights();

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
    m_ssao->bind(2);

    m_compositeShader->uniform("colorInput", 0);
    m_compositeShader->uniform("brightInput", 1);
    m_compositeShader->uniform("ssaoSampler", 2);

    m_compositeShader->uniform("bloom", m_bloom);
    m_compositeShader->uniform("useSSAO", m_useSSAO);

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

void PBRScene::RenderLights()
{
    m_lightShader->use();
    m_lightShader->uniform("projection", m_projection);
    m_lightShader->uniform("tonemap", m_tonemap);
    m_lightShader->uniform("view", m_camera.viewMatrix());

    for (unsigned int i = 0; i < m_howManyLights; i++) {
        glm::vec3 position = m_lights[i].position;
        glm::vec3 color = m_lights[i].color;

        glm::mat4 model;
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(0.1f));

        m_lightShader->uniform("model", model);
        m_lightShader->uniform("lightColor", color);


        m_cube->draw();
    }
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
