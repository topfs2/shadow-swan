#include "Cubemap.h"
#include <string>
#include <cmath>
#include <stb_image.h>

Cubemap::Cubemap(const std::string &prefix, const std::string &suffix, bool mipmap) : m_texture(0) {
    refresh(prefix, suffix, mipmap);
}

Cubemap::Cubemap(const std::string &prefix, const std::string &suffix, int mipmaps) : m_texture(0) {
    refresh(prefix, suffix, mipmaps);
}

Cubemap::Cubemap(std::vector<std::string> paths) : m_texture(0) {
    refresh(paths);
}

Cubemap::~Cubemap() {
    glDeleteTextures(1, &m_texture);
}

void Cubemap::bind(unsigned int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
}

void Cubemap::unbind(unsigned int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::refresh(const std::string &prefix, const std::string &suffix, bool mipmap)
{
    refresh({
        prefix + "posx" + suffix,
        prefix + "negx" + suffix,
        prefix + "posy" + suffix,
        prefix + "negy" + suffix,
        prefix + "posz" + suffix,
        prefix + "negz" + suffix
    });

    if (mipmap) {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}

void Cubemap::refresh(const std::string &prefix, const std::string &suffix, int mipmaps)
{
    std::vector<std::string> paths;

    std::string order[6] = {
        "posx",
        "negx",
        "posy",
        "negy",
        "posz",
        "negz"
    };

    for (unsigned int i = 0; i < 6; i++) {
        for (unsigned int j = 0; j <= mipmaps; j++) {
            int size = std::pow(2, mipmaps - j);
            std::string resolution =  std::to_string(size)+ "x" + std::to_string(size);
            std::string path = prefix + order[i] + "_" + std::to_string(j) + "_" + resolution + suffix;
            paths.push_back(path);
        }
    }

    refresh(paths);
}

void Cubemap::refresh(std::vector<std::string> paths)
{
    stbi_set_flip_vertically_on_load(false);

    if (m_texture == 0) {
        glGenTextures(1, &m_texture);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
    unsigned int levels = paths.size() / 6;

    for (unsigned int i = 0; i < 6; i++) {
        for (unsigned int level = 0; level < levels; level++) {
            unsigned int index = level + levels * i;
            std::string path = paths[index];

            int width, height, nrComponents;
            float *data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, level, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
