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
    explicit Model(const std::string& path);

    void draw(const Shader& shader);

private:
    std::vector<Mesh> meshes_;
};

} // namespace rg

#endif // RG_RENDERER_MODEL_MODEL_HPP
