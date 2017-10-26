#pragma once
#include <GL/glew.h>
#include <memory>
#include <map>
#include <vector>
#include "Image.h"

class FrameBuffer {
public:
    typedef std::vector<ImagePtr> ImageVector;

    FrameBuffer(unsigned int width, unsigned int height);
    FrameBuffer(ImagePtr colorAttachment, unsigned int width, unsigned int height);
    FrameBuffer(ImageVector colorAttachments, unsigned int width, unsigned int height);
    FrameBuffer(ImagePtr colorAttachment, ImagePtr depthAttachment, unsigned int width, unsigned int height);
    FrameBuffer(ImageVector colorAttachments, ImagePtr depthAttachment, unsigned int width, unsigned int height);
    virtual ~FrameBuffer();

    unsigned int getWidth() const;
    unsigned int getHeight() const;

    void bind();
    static void unbind();

    ImagePtr getColorAttachment(unsigned int attachment = 0);
    ImagePtr getDepthAttachment();

private:
    void setup(ImageVector colorAttachments, ImagePtr depthAttachment, unsigned int width, unsigned int height);

    GLuint m_framebuffer;
    unsigned int m_width;
    unsigned int m_height;

    GLuint m_rbo;

    typedef std::map<GLenum, ImagePtr> AttachmentMap;
    AttachmentMap m_colorAttachments;

    ImagePtr m_depthAttachment;
};

typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;
