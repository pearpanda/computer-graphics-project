#ifndef RG_RENDERER_BUFFER_VERTEXARRAY_HPP
#define RG_RENDERER_BUFFER_VERTEXARRAY_HPP

#include "VertexBuffer.hpp"
#include "VertexLayout.hpp"

namespace rg {

class VertexArray {
public:
    VertexArray();
    VertexArray(const VertexArray& va) = delete;
    VertexArray operator=(const VertexArray& va) = delete;
    VertexArray(VertexArray&& va) noexcept;
    VertexArray& operator=(VertexArray&& va) noexcept;
    ~VertexArray();
    void bind() const;
    void unbind() const;
    void recordLayout(const VertexBuffer& vb, const VertexLayout& layout) const;

private:
    unsigned int array_id_;
};

} // namespace rg

#endif // RG_RENDERER_BUFFER_VERTEXARRAY_HPP
