#pragma once
#include <GL/glew.h>
#include <memory>

class Image {
public:
    Image(const std::string &path, bool mipmap = false);
    Image(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format);
    Image(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, const void *data, bool mipmap);
    Image(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, const void *data, bool mipmap, GLenum minFilter, GLenum maxFilter, GLenum wrapS, GLenum wrapT);
    virtual ~Image();

    void refresh(const std::string &path, bool mipmap = false);
    void refresh(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, const void *data, bool mipmap);
    void refresh(unsigned int width, unsigned int height, GLenum internalFormat, GLenum format, GLenum type, const void *data, bool mipmap, GLenum minFilter, GLenum maxFilter, GLenum wrapS, GLenum wrapT);

    void bind(unsigned int unit);
    static void unbind(unsigned int unit);

    void attach(GLenum attachment);

private:
    GLuint m_texture;
};

typedef std::shared_ptr<Image> ImagePtr;
