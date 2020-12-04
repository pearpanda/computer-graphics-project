#include <rg/renderer/buffer/IndexBuffer.hpp>

#include <glad/glad.h>

namespace rg {

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) {
    bufferID = 0;
    indices = count;
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof (*data), data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(const std::vector<unsigned int>& data) {
    bufferID = 0;
    indices = data.size();
    glGenBuffers(1, &bufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof (data.front()), data.data(), GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &bufferID);
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0U);
}

unsigned int IndexBuffer::count() const {
    return indices;
}

} // namespace rg
