#ifndef RG_RENDERER_BUFFER_VERTEXLAYOUT_HPP
#define RG_RENDERER_BUFFER_VERTEXLAYOUT_HPP

#include <initializer_list>
#include <vector>

namespace rg {

enum class ElementType { FLOAT, UNSIGNED_INT };
struct LayoutElement;
class VertexLayout;

namespace util {

std::size_t size(const ElementType& type);
unsigned int intValue(const ElementType& type);

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
    std::vector<LayoutElement> elements_;
    unsigned int stride_;
};

} // namespace rg

#endif // RG_RENDERER_BUFFER_VERTEXLAYOUT_HPP
