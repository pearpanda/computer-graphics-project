#include <rg/renderer/buffer/FrameBuffer.hpp>

#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace rg {

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height,
                         bool enable_depth_stencil_sampling)
        : framebuffer_id_{0}, color_id_{0} {
    glGenFramebuffers(1, &framebuffer_id_);
    this->bind();

    // Color texture
    glGenTextures(1, &color_id_);
    glBindTexture(GL_TEXTURE_2D, color_id_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Bind color texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           color_id_, 0);

    // If sampling is not required, then, the depth and stencil values are only
    // written to or tested (depth and stencil tests). In such cases, render
    // buffers are more efficient than textures, due to GPU optimizations.
    // Otherwise, we simply have to use textures.
    if (enable_depth_stencil_sampling) {
        unsigned int depth_stencil_texture;
        glGenTextures(1, &depth_stencil_texture);
        glBindTexture(GL_TEXTURE_2D, depth_stencil_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
                     GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

        // Bind depth and stencil texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                               GL_TEXTURE_2D, depth_stencil_texture, 0);
    } else {
        unsigned int depth_stencil_render_buffer;
        glGenRenderbuffers(1, &depth_stencil_render_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_render_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width,
                              height);

        // Bind depth and stencil texture to framebuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                  GL_RENDERBUFFER, depth_stencil_render_buffer);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("ERROR::RG::FRAMEBUFFER: {}",
                      "Framebuffer creation failed");
    }

    this->unbind();
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &framebuffer_id_);
    framebuffer_id_ = 0;
}

unsigned int FrameBuffer::get_color_texture() const {
    return color_id_;
}

void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id_);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void FrameBuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace rg
