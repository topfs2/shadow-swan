#include "Cubemap.h"
#include <string>
#include <stb_image.h>

Cubemap::Cubemap(const std::string &prefix, const std::string &suffix, bool mipmap) : m_texture(0) {
    refresh(prefix, suffix, mipmap);
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
    stbi_set_flip_vertically_on_load(false);

    if (m_texture == 0) {
        glGenTextures(1, &m_texture);
    }

    std::string order[6] = {
        "posx",
        "negx",
        "posy",
        "negy",
        "posz",
        "negz"
    };

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
    for(GLuint i = 0; i < 6; i++) {
        std::string path = prefix + order[i] + suffix;
        int width, height, nrComponents;
        float *data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, mipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    if (mipmap) {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}
