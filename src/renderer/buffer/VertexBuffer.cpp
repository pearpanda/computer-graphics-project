#include <rg/renderer/buffer/VertexBuffer.hpp>

#include <glad/glad.h>

namespace rg {

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
    bufferID = 0;
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &bufferID);
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0U);
}

} // namespace rg
