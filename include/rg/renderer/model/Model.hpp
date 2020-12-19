#ifndef RG_RENDERER_MODEL_MODEL_HPP
#define RG_RENDERER_MODEL_MODEL_HPP

#include <rg/renderer/model/Mesh.hpp>
#include <rg/renderer/model/Texture.hpp>
#include <rg/renderer/shader/Shader.hpp>

#include <string>
#include <vector>

namespace rg {

class Model {
public:
    explicit Model(std::string name, const std::string& path);

    void draw(const Shader& shader);

    [[nodiscard]] std::string get_name() const;

private:
    std::string name_;
    std::vector<Mesh> meshes_;
};

} // namespace rg

#endif // RG_RENDERER_MODEL_MODEL_HPP
