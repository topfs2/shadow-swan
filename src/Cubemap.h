#pragma once
#include <GL/glew.h>
#include <memory>
#include <string>

class Cubemap {
public:
    Cubemap(const std::string &prefix, const std::string &suffix, bool mipmap = false);
    virtual ~Cubemap();

    void refresh(const std::string &prefix, const std::string &suffix, bool mipmap = false);

    void bind(unsigned int unit);
    static void unbind(unsigned int unit);

private:
    GLuint m_texture;
};

typedef std::shared_ptr<Cubemap> CubemapPtr;
