#include "FrameBuffer.h"
#include <iostream>

using namespace std;

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) : m_framebuffer(0), m_width(width), m_height(height), m_rbo(0) {
    ImagePtr def(new Image(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, NULL, false));
    setup({ def }, ImagePtr(), width, height);
}

FrameBuffer::FrameBuffer(ImagePtr colorAttachment, unsigned int width, unsigned int height) : m_framebuffer(0), m_width(width), m_height(height), m_rbo(0) {
    setup({ colorAttachment }, ImagePtr(), width, height);
}

FrameBuffer::FrameBuffer(FrameBuffer::ImageVector colorAttachments, unsigned int width, unsigned int height) : m_framebuffer(0), m_width(width), m_height(height), m_rbo(0) {
    setup(colorAttachments, ImagePtr(), width, height);
}

FrameBuffer::FrameBuffer(ImagePtr colorAttachment, ImagePtr depthAttachment, unsigned int width, unsigned int height) : m_framebuffer(0), m_width(width), m_height(height), m_rbo(0) {
    if (colorAttachment) {
        setup({ colorAttachment }, depthAttachment, width, height);
    } else {
        setup({ }, depthAttachment, width, height);
    }
}

FrameBuffer::FrameBuffer(FrameBuffer::ImageVector colorAttachments, ImagePtr depthAttachment, unsigned int width, unsigned int height) : m_framebuffer(0), m_width(width), m_height(height), m_rbo(0) {
    setup(colorAttachments, depthAttachment, width, height);
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &m_framebuffer);
    glDeleteRenderbuffers(1, &m_rbo);
}

unsigned int FrameBuffer::getWidth() const {
    return m_width;
}

unsigned int FrameBuffer::getHeight() const {
    return m_height;
}

void FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ImagePtr FrameBuffer::getColorAttachment(unsigned int attachment) {
    return m_colorAttachments[GL_COLOR_ATTACHMENT0 + attachment];
}

ImagePtr FrameBuffer::getDepthAttachment() {
    return m_depthAttachment;
}

void FrameBuffer::setup(FrameBuffer::ImageVector colorAttachments, ImagePtr depthAttachment, unsigned int width, unsigned int height) {
    m_width = width;
    m_height = height;

    if (m_framebuffer == 0) {
        glGenFramebuffers(1, &m_framebuffer);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    if (colorAttachments.size() > 0) {
        GLuint buffers[colorAttachments.size()];

        for (unsigned int i = 0; i < colorAttachments.size(); i++) {
            GLenum attachment = GL_COLOR_ATTACHMENT0 + i;

            colorAttachments[i]->attach(attachment);
            m_colorAttachments[attachment] = colorAttachments[i];
            buffers[i] = attachment;
        }

        glDrawBuffers(colorAttachments.size(), buffers);
    } else {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    if (depthAttachment) {
        depthAttachment->attach(GL_DEPTH_ATTACHMENT);
        m_depthAttachment = depthAttachment;
    } else {
        // Generate depth/stencil
        if (m_rbo == 0) {
            glGenRenderbuffers(1, &m_rbo);
        }

        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
