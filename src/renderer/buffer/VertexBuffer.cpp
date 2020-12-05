#include <rg/renderer/buffer/VertexBuffer.hpp>

#include <glad/glad.h>

namespace rg {

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    buffer_id_ = 0;
    glGenBuffers(1, &buffer_id_);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &buffer_id_);
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0U);
}

} // namespace rg
