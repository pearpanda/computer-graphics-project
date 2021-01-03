#include <rg/renderer/buffer/FrameBuffer.hpp>

#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace rg {

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height,
                         bool enable_depth_stencil_sampling)
        : framebuffer_id_{0}, intermediate_framebuffer_id_{0},
          screen_texture_{0}, width{width}, height{height} {
    glGenFramebuffers(1, &framebuffer_id_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id_);

    // Color texture
    glGenTextures(1, &texture_color_buffer_multisampled_);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,
                  texture_color_buffer_multisampled_);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, width, height,
                            GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Bind color texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D_MULTISAMPLE,
                           texture_color_buffer_multisampled_, 0);

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
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4,
                                         GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // Bind depth and stencil texture to framebuffer
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                  GL_RENDERBUFFER, depth_stencil_render_buffer);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("ERROR::RG::FRAMEBUFFER: {}",
                      "Framebuffer creation failed");
    }
    this->unbind();

    glGenFramebuffers(1, &intermediate_framebuffer_id_);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediate_framebuffer_id_);
    glGenTextures(1, &screen_texture_);
    glBindTexture(GL_TEXTURE_2D, screen_texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           screen_texture_, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("ERROR::RG::FRAMEBUFFER:: Intermediate framebuffer "
                      "creation failed!");
    }
    this->unbind();
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &framebuffer_id_);
    glDeleteFramebuffers(1, &intermediate_framebuffer_id_);
    framebuffer_id_ = 0;
    intermediate_framebuffer_id_ = 0;
}

unsigned int FrameBuffer::get_color_texture() const {
    return screen_texture_;
}

void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id_);
}

void FrameBuffer::blit() const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer_id_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediate_framebuffer_id_);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
};

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void FrameBuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace rg
