#pragma once
#include "BaseScene.h"
#include "../Cubemap.h"
#include "../Image.h"
#include "../FrameBuffer.h"
#include "../Geometry.h"
#include "../Mesh.h"
#include "../Shader.h"

class PBRScene : public BaseScene
{
public:
    PBRScene();
    virtual ~PBRScene();

    virtual void OnResize(unsigned int width, unsigned int height);
    virtual void OnKey(int key, int scancode, int action, int mode);
    virtual void OnRender(float t, float dt);

private:
    void RenderSkybox(CubemapPtr skybox, int tonemap);
    void RenderLights(int tonemap);
    void RenderGeometries(ShaderPtr shader, glm::mat4 view, glm::mat4 projection, GLuint unit, int whichMesh);

    struct IBL {
        CubemapPtr skybox;
        CubemapPtr irradiance;
        CubemapPtr radiance;
    };

    struct Light {
        Light(glm::vec3 _1, glm::vec3 _2, FrameBufferPtr _3) : position(_1), color(_2), framebuffer(_3), active(true) { }

        glm::vec3 position;
        glm::vec3 color;
        FrameBufferPtr framebuffer;
        bool active;
    };

    static IBL loadIBL(std::string name);

    ShaderPtr m_shadowDepthShader;
    ShaderPtr m_geometryShader;
    ShaderPtr m_ssaoShader;
    ShaderPtr m_ssaoBlurShader;
    ShaderPtr m_pbrShader;
    ShaderPtr m_brightShader;
    ShaderPtr m_blurShader;
    ShaderPtr m_compositeShader;

    ShaderPtr m_skyboxShader;
    ShaderPtr m_lightShader;

    std::vector<glm::vec3> m_ssaoKernel;
    ImagePtr m_noiseTexture;
    glm::vec2 m_noiseScale;

    unsigned int m_width;
    unsigned int m_height;

    std::vector<IBL> m_ibls;
    ImagePtr m_brdf;

    std::vector<Light> m_lights;

    ImagePtr m_gNormal;
    ImagePtr m_gPosition;
    FrameBufferPtr m_geometryFramebuffer;

    ImagePtr m_ssaoInput;
    FrameBufferPtr m_ssaoFrameBuffer;

    ImagePtr m_ssao;
    FrameBufferPtr m_ssaoBlurFrameBuffer;

    ImagePtr m_gColorOutput;
    FrameBufferPtr m_colorFramebuffer;

    ImagePtr m_gPingOutput;
    FrameBufferPtr m_pingFramebuffer;

    ImagePtr m_gPongOutput;
    FrameBufferPtr m_pongFramebuffer;

    GeometryPtr m_fsQuad;
    GeometryPtr m_cube;

    std::vector<MeshPtr> m_meshes;
    MeshPtr m_ground;
};
