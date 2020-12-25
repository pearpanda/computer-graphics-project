#include <rg/renderer/buffer/IndexBuffer.hpp>

#include <glad/glad.h>

namespace rg {

IndexBuffer::IndexBuffer() : buffer_id_{0}, indices_{0} {
}

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) {
    buffer_id_ = 0;
    indices_ = count;
    glGenBuffers(1, &buffer_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(*data), data,
                 GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(const std::vector<unsigned int>& data) {
    buffer_id_ = 0;
    indices_ = data.size();
    glGenBuffers(1, &buffer_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(data.front()),
                 data.data(), GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(IndexBuffer&& ib) noexcept
        : buffer_id_{ib.buffer_id_}, indices_{ib.indices_} {
    ib.buffer_id_ = 0;
    ib.indices_ = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& ib) noexcept {
    this->buffer_id_ = ib.buffer_id_;
    this->indices_ = ib.indices_;
    ib.buffer_id_ = 0;
    ib.indices_ = 0;
    return (*this);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &buffer_id_);
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0U);
}

unsigned int IndexBuffer::count() const {
    return indices_;
}

} // namespace rg
