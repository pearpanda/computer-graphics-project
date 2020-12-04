#ifndef RG_RENDERER_BUFFER_VERTEXLAYOUT_HPP
#define RG_RENDERER_BUFFER_VERTEXLAYOUT_HPP

#include <vector>
#include <initializer_list>

namespace rg {

enum class ElementType { FLOAT, UNSIGNED_INT };
struct LayoutElement;
class VertexLayout;

namespace util {

constexpr std::size_t size(const ElementType& type);
LayoutElement floatVector(unsigned int count);
LayoutElement uintVector(unsigned int count);

} // namespace util

struct LayoutElement {
    ElementType type;
    unsigned int count;
    bool normalized;

    LayoutElement(ElementType type, unsigned int count, bool normalized);
    [[nodiscard]] std::size_t size() const;
};

class VertexLayout {
public:
    VertexLayout();
    explicit VertexLayout(std::vector<LayoutElement> elements);
    VertexLayout(std::initializer_list<LayoutElement> elements);

    void push(LayoutElement element);
    [[nodiscard]] std::vector<unsigned int> offsets() const;
    [[nodiscard]] unsigned int stride() const;
    [[nodiscard]] const std::vector<LayoutElement>& get_elements() const;
private:
    std::vector<LayoutElement> elements;
    unsigned int currentStride;
};

} // namespace rg

#endif // RG_RENDERER_BUFFER_VERTEXLAYOUT_HPP
