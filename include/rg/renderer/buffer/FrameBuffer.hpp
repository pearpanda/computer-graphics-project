#ifndef RG_RENDERER_BUFFER_FRAMEBUFFER_HPP
#define RG_RENDERER_BUFFER_FRAMEBUFFER_HPP

namespace rg {

class FrameBuffer {
public:
    FrameBuffer(unsigned int width, unsigned int height,
                bool enable_depth_stencil_sampling = false);
    ~FrameBuffer();
    void bind() const;
    void unbind() const;
    unsigned int get_color_texture() const;

private:
    unsigned int framebuffer_id_;
    unsigned int color_id_;
};

} // namespace rg

#endif // RG_RENDERER_BUFFER_FRAMEBUFFER_HPP
