#ifndef RG_RENDERER_BUFFER_INDEXBUFFER_HPP
#define RG_RENDERER_BUFFER_INDEXBUFFER_HPP

#include <vector>

namespace rg {

class IndexBuffer {
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    explicit IndexBuffer(const std::vector<unsigned int>& data);
    ~IndexBuffer();
    void bind() const;
    void unbind() const;
    [[nodiscard]] unsigned int count() const;

private:
    unsigned int bufferID;
    unsigned int indices;
};

} // namespace rg

#endif // RG_RENDERER_BUFFER_INDEXBUFFER_HPP
