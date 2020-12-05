#ifndef RG_RENDERER_BUFFER_VERTEXBUFFER_HPP
#define RG_RENDERER_BUFFER_VERTEXBUFFER_HPP

namespace rg {

class VertexBuffer {
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();
    void bind() const;
    void unbind() const;

private:
    unsigned int buffer_id_;
};

} // namespace rg

#endif // RG_RENDERER_BUFFER_VERTEXBUFFER_HPP