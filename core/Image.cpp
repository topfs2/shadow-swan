#include "Image.h"
#include <stb_image.h>

Image::Image(const std::string &path, bool mipmap) : m_texture(0) {
    refresh(path, mipmap);
}

Image::Image(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format) : m_texture(0) {
    refresh(width, height, internalFormat, format, GL_UNSIGNED_BYTE, NULL, false);
}

Image::Image(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, const void *data, bool mipmap) : m_texture(0) {
    refresh(width, height, internalFormat, format, type, data, mipmap);
}

Image::Image(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, const void *data, bool mipmap, GLenum minFilter, GLenum maxFilter, GLenum wrapS, GLenum wrapT) : m_texture(0) {
    refresh(width, height, internalFormat, format, type, data, mipmap, minFilter, maxFilter, wrapS, wrapT);
}

Image::~Image() {
    glDeleteTextures(1, &m_texture);
}

void Image::refresh(const std::string &path, bool mipmap)
{
    stbi_set_flip_vertically_on_load(false);
    int width, height, bpp;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &bpp, 4);

    if (data) {
        refresh(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, data, mipmap);
        stbi_image_free(data);
    }
}

void Image::refresh(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, const void *data, bool mipmap) {
    refresh(width, height, internalFormat, format, type, data, mipmap, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
}

void Image::refresh(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, const void *data, bool mipmap, GLenum minFilter, GLenum maxFilter, GLenum wrapS, GLenum wrapT) {
    if (m_texture == 0) {
        glGenTextures(1, &m_texture);
    }

    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, maxFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    if (mipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Image::bind(unsigned int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Image::unbind(unsigned int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Image::attach(GLenum attachment) {
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, m_texture, 0);
}
