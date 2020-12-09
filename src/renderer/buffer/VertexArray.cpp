#include <rg/renderer/buffer/VertexArray.hpp>

#include <glad/glad.h>

namespace rg {

VertexArray::VertexArray() {
    array_id_ = 0;
    glGenVertexArrays(1, &array_id_);
    glBindVertexArray(array_id_);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &array_id_);
}

VertexArray::VertexArray(VertexArray&& va) noexcept : array_id_{va.array_id_} {
    va.array_id_ = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& va) noexcept {
    this->array_id_ = va.array_id_;
    va.array_id_ = 0;
    return (*this);
}

void VertexArray::bind() const {
    glBindVertexArray(array_id_);
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
    unsigned int element_count = elements.size();
    auto offsets = layout.offsets();

    for (unsigned int i = 0; i < element_count; ++i) {
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
