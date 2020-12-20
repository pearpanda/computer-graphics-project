#ifndef RG_RENDERER_MODEL_SKYBOX_HPP
#define RG_RENDERER_MODEL_SKYBOX_HPP

#include <rg/renderer/model/Cubemap.hpp>
#include <rg/renderer/model/Mesh.hpp>
#include <rg/renderer/shader/Shader.hpp>

#include <memory>
#include <vector>

namespace rg {

class Skybox {
public:
    Skybox(const std::string& path, const std::vector<std::string>& faces);
    Skybox(const std::string& path, const std::vector<std::string>& faces,
           std::shared_ptr<MeshVertexData> cube);

    void draw(const Shader& shader) const;

private:
    std::shared_ptr<MeshVertexData> cube_;
    Cubemap cubemap_;
};

} // namespace rg

#endif // RG_RENDERER_MODEL_SKYBOX_HPP
