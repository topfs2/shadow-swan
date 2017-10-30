#pragma once
#include <GL/glew.h>
#include <memory>
#include <string>
#include <vector>

class Cubemap {
public:
    Cubemap(const std::string &prefix, const std::string &suffix, bool mipmap = false);
    Cubemap(const std::string &prefix, const std::string &suffix, int mipmaps);
    Cubemap(std::vector<std::string> paths);
    virtual ~Cubemap();

    void refresh(const std::string &prefix, const std::string &suffix, bool mipmap = false);
    void refresh(const std::string &prefix, const std::string &suffix, int mipmaps);
    void refresh(std::vector<std::string> paths);

    void bind(unsigned int unit);
    static void unbind(unsigned int unit);

private:
    GLuint m_texture;
};

typedef std::shared_ptr<Cubemap> CubemapPtr;
