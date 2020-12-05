#include <rg/renderer/buffer/VertexArray.hpp>

#include <glad/glad.h>

namespace rg {

VertexArray::VertexArray() {
    arrayID = 0;
    glGenVertexArrays(1, &arrayID);
    glBindVertexArray(arrayID);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &arrayID);
}

void VertexArray::bind() const {
    glBindVertexArray(arrayID);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void VertexArray::unbind() const {
    glBindVertexArray(0);
}

void VertexArray::recordLayout(const VertexBuffer& vb,
                               const VertexLayout& layout) const {
    bind();
    vb.bind();

    const auto& elements = layout.get_elements();
    unsigned int elementCount = elements.size();
    auto offsets = layout.offsets();

    for (unsigned int i = 0; i < elementCount; ++i) {
        const auto& e = elements[i];
        unsigned long long offset = offsets[i];
        unsigned int type_id = util::intValue(e.type);
        glVertexAttribPointer(i, e.count, type_id,
                              (e.normalized ? GL_TRUE : GL_FALSE),
                              layout.stride(),
                              reinterpret_cast<const void*>(offset));
        glEnableVertexAttribArray(i);
    }
}

} // namespace rg
