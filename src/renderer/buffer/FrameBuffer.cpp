#include <rg/renderer/buffer/FrameBuffer.hpp>

#include <glad/glad.h>
#include <spdlog/spdlog.h>

namespace rg {

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
        : framebuffer_id_{0}, intermediate_framebuffer_id_{0},
          screen_color_texture_id_{0}, width_{width}, height_{height} {
    // screen_color_texture_id_ is kinda like old color_id_ (it's the one being
    // drawn)
    glGenFramebuffers(1, &framebuffer_id_);
    this->bind();

    // Color texture
    unsigned int multisampledColorTexture;
    glGenTextures(1, &multisampledColorTexture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisampledColorTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA_SAMPLES, GL_RGB,
                            width, height, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Bind color texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D_MULTISAMPLE, multisampledColorTexture,
                           0);

    unsigned int depth_stencil_render_buffer;
    glGenRenderbuffers(1, &depth_stencil_render_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_stencil_render_buffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA_SAMPLES,
                                     GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Bind depth and stencil texture to framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, depth_stencil_render_buffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("ERROR::RG::FRAMEBUFFER: {}",
                      "Framebuffer creation failed");
    }
    this->unbind();

    // initialize intermediate framebuffer: the one which is not multisampled
    // and which is actually drawn to the screen
    glGenFramebuffers(1, &intermediate_framebuffer_id_);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediate_framebuffer_id_);
    glGenTextures(1, &screen_color_texture_id_);
    glBindTexture(GL_TEXTURE_2D, screen_color_texture_id_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           screen_color_texture_id_, 0);

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
    return screen_color_texture_id_;
}

void FrameBuffer::bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id_);
}

void FrameBuffer::blit() const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer_id_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediate_framebuffer_id_);
    glBlitFramebuffer(0, 0, width_, height_, 0, 0, width_, height_,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void FrameBuffer::unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

} // namespace rg
