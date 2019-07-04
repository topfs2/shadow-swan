#pragma once
#include <glad/glad.h>
#include <gli/gli.hpp>

struct Texture {
    GLenum target;
    GLuint texture;

    Texture(GLenum _target, GLuint _texture) : target(_target), texture(_texture) {
        assert(isValid());
    }

    void bind(unsigned int unit) {
        assert(isValid());

        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(target, texture);
    }

    void unbind(unsigned int unit) {
        assert(target != 0);

        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(target, 0);
    }

    void attach(GLenum attachment) {
        assert(target == GL_TEXTURE_2D);
        assert(isValid());

        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture, 0);
    }

    bool isValid() const {
    	return target != 0 && texture != 0;
    }

    static std::shared_ptr<Texture> create(const gli::texture &texture);
    static std::shared_ptr<Texture> create(const std::string &filename);
};

typedef std::shared_ptr<Texture> TexturePtr;
