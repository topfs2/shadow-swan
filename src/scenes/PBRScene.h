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
    void RenderSkybox(CubemapPtr skybox);
    void RenderLamp(glm::vec3 position, glm::vec3 color);

    struct IBL {
        CubemapPtr skybox;
        CubemapPtr irradiance;
        CubemapPtr radiance;
    };

    static IBL loadIBL(std::string name);

    int m_tonemap;
    bool m_bloom;
    bool m_useIBL;
    int m_whichIBL;
    int m_lights;
    bool m_useCheapIBL;

    ShaderPtr m_pbrShader;
    ShaderPtr m_brightShader;
    ShaderPtr m_blurShader;
    ShaderPtr m_compositeShader;

    ShaderPtr m_skyboxShader;
    ShaderPtr m_lampShader;

    std::vector<IBL> m_ibls;
    ImagePtr m_brdf;

    ImagePtr m_gColorOutput;
    FrameBufferPtr m_colorFramebuffer;

    ImagePtr m_gPingOutput;
    FrameBufferPtr m_pingFramebuffer;

    ImagePtr m_gPongOutput;
    FrameBufferPtr m_pongFramebuffer;

    GeometryPtr m_fsQuad;
    GeometryPtr m_cube;

    MeshPtr m_helmet;
};
