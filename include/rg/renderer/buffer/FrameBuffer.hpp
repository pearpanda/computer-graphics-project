#ifndef RG_RENDERER_BUFFER_FRAMEBUFFER_HPP
#define RG_RENDERER_BUFFER_FRAMEBUFFER_HPP

namespace rg {

class FrameBuffer {
public:
    FrameBuffer(unsigned int width, unsigned int height,
                bool enable_depth_stencil_sampling = false);
    ~FrameBuffer();
    /**
     * Bind the (multisampled) framebuffer.
     */
    void bind() const;
    /**
     * Copy from the multisampled framebuffer to the regular one, resolving
     * colors automatically. After copying, the regular framebuffer is the one
     * being drawn. Calling this method will unbind all framebuffers afterwards,
     * so you can draw directly to the screen.
     */
    void blit() const;
    void unbind() const;
    /**
     * id of the color texture for the regular (non-multisampled) framebuffer.
     * Only valid after calling blit().
     * @return texture id
     */
    unsigned int get_color_texture() const;

    static constexpr unsigned int MSAA_SAMPLES = 4;

private:
    unsigned int framebuffer_id_;
    unsigned int intermediate_framebuffer_id_;
    unsigned int screen_color_texture_;

    unsigned int width, height;
};

} // namespace rg

#endif // RG_RENDERER_BUFFER_FRAMEBUFFER_HPP
