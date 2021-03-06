#ifndef RG_RENDERER_BUFFER_INDEXBUFFER_HPP
#define RG_RENDERER_BUFFER_INDEXBUFFER_HPP

#include <vector>

namespace rg {

class IndexBuffer {
public:
    IndexBuffer();
    IndexBuffer(const unsigned int* data, unsigned int count);
    explicit IndexBuffer(const std::vector<unsigned int>& data);
    IndexBuffer(const IndexBuffer& ib) = delete;
    IndexBuffer operator=(const IndexBuffer& ib) = delete;
    IndexBuffer(IndexBuffer&& ib) noexcept;
    IndexBuffer& operator=(IndexBuffer&& ib) noexcept;
    ~IndexBuffer();
    void bind() const;
    void unbind() const;
    [[nodiscard]] unsigned int count() const;

private:
    unsigned int buffer_id_;
    unsigned int indices_;
};

} // namespace rg

#endif // RG_RENDERER_BUFFER_INDEXBUFFER_HPP
