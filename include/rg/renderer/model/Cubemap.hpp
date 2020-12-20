#ifndef RG_RENDERER_MODEL_CUBEMAP_HPP
#define RG_RENDERER_MODEL_CUBEMAP_HPP

#include <string>
#include <vector>

namespace rg {

class Cubemap {
public:
    Cubemap();
    Cubemap(const std::string& path, const std::vector<std::string>& faces);
    ~Cubemap();

    Cubemap(const Cubemap& other) = delete;
    Cubemap operator=(const Cubemap& other) = delete;
    Cubemap(Cubemap&& other) noexcept;
    Cubemap& operator=(Cubemap&& other) noexcept;

    unsigned int get_cubemap_texture() const;
    void bind() const;
    void unbind() const;

private:
    unsigned int texture_id_;
};

} // namespace rg

#endif // RG_RENDERER_MODEL_CUBEMAP_HPP
