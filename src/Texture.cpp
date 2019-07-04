#include "Texture.h"

std::shared_ptr<Texture> Texture::create(const gli::texture &texture) {
    if (texture.empty()) {
        return nullptr;
    }

    gli::gl GL(gli::gl::PROFILE_GL33);
    gli::gl::format const formatGL = GL.translate(texture.format(), texture.swizzles());
    GLenum targetGL = GL.translate(texture.target());

    GLuint textureGL = 0;
    glGenTextures(1, &textureGL);
    glBindTexture(targetGL, textureGL);
    glTexParameteri(targetGL, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(targetGL, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(texture.levels() - 1));
    glTexParameteri(targetGL, GL_TEXTURE_SWIZZLE_R, formatGL.Swizzles[0]);
    glTexParameteri(targetGL, GL_TEXTURE_SWIZZLE_G, formatGL.Swizzles[1]);
    glTexParameteri(targetGL, GL_TEXTURE_SWIZZLE_B, formatGL.Swizzles[2]);
    glTexParameteri(targetGL, GL_TEXTURE_SWIZZLE_A, formatGL.Swizzles[3]);

    glm::tvec3<GLsizei> const extent(texture.extent());
    GLsizei const faceTotal = static_cast<GLsizei>(texture.layers() * texture.faces());

    for (std::size_t layer = 0; layer < texture.layers(); ++layer) {
        for (std::size_t face = 0; face < texture.faces(); ++face) {
            for (std::size_t level = 0; level < texture.levels(); ++level) {
                GLsizei const layerGL = static_cast<GLsizei>(layer);
                glm::tvec3<GLsizei> extent(texture.extent(level));
                targetGL = gli::is_target_cube(texture.target())
                    ? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face)
                    : targetGL;

                switch (texture.target()) {
                    case gli::TARGET_1D:
                        if (gli::is_compressed(texture.format())) {
                            glCompressedTexImage1D(
                                targetGL, static_cast<GLint>(level),
                                formatGL.Internal,
                                extent.x,
                                0,
                                static_cast<GLsizei>(texture.size(level)),
                                texture.data(layer, face, level));
                        } else {
                            glTexImage1D(
                                targetGL, static_cast<GLint>(level),
                                formatGL.Internal,
                                extent.x,
                                0,
                                formatGL.External, formatGL.Type,
                                texture.data(layer, face, level));
                        }
                        break;
                    case gli::TARGET_1D_ARRAY:
                    case gli::TARGET_2D:
                    case gli::TARGET_CUBE:
                        if (gli::is_compressed(texture.format())) {
                            glCompressedTexImage2D(
                                targetGL, static_cast<GLint>(level),
                                formatGL.Internal,
                                extent.x,
                                texture.target() == gli::TARGET_1D_ARRAY ? layerGL : extent.y,
                                0,
                                static_cast<GLsizei>(texture.size(level)),
                                texture.data(layer, face, level));
                        } else {
                            glTexImage2D(
                                targetGL, static_cast<GLint>(level),
                                formatGL.Internal,
                                extent.x,
                                texture.target() == gli::TARGET_1D_ARRAY ? layerGL : extent.y,
                                0,
                                formatGL.External, formatGL.Type,
                                texture.data(layer, face, level));
                        }
                        break;
                    case gli::TARGET_2D_ARRAY:
                    case gli::TARGET_3D:
                    case gli::TARGET_CUBE_ARRAY:
                        if (gli::is_compressed(texture.format())) {
                            glCompressedTexImage3D(
                                targetGL, static_cast<GLint>(level),
                                formatGL.Internal,
                                extent.x, extent.y,
                                texture.target() == gli::TARGET_3D ? extent.z : layerGL,
                                0,
                                static_cast<GLsizei>(texture.size(level)),
                                texture.data(layer, face, level));
                        } else {
                            glTexImage3D(
                                targetGL, static_cast<GLint>(level),
                                formatGL.Internal,
                                extent.x, extent.y,
                                texture.target() == gli::TARGET_3D ? extent.z : layerGL,
                                0,
                                formatGL.External, formatGL.Type,
                                texture.data(layer, face, level));
                        }
                        break;
                    default:
                        assert(0);
                        break;
                }
            }
        }
    }

    return std::make_shared<Texture>(targetGL, textureGL);
}


std::shared_ptr<Texture> Texture::create(const std::string &filename) {
    // TODO handle stb_image formats
    auto texture = gli::load(filename);
    return create(texture);
}
