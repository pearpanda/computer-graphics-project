#ifndef RG_RENDERER_MODEL_MESH_HPP
#define RG_RENDERER_MODEL_MESH_HPP

#include <rg/renderer/buffer/IndexBuffer.hpp>
#include <rg/renderer/buffer/VertexArray.hpp>
#include <rg/renderer/buffer/VertexBuffer.hpp>
#include <rg/renderer/model/Texture.hpp>
#include <rg/renderer/shader/Shader.hpp>

#include <memory>

namespace rg {

struct MeshVertexData {
    VertexArray vertex_array;
    VertexBuffer vertex_buffer;
    IndexBuffer index_buffer;
};

class Mesh {
public:
    Mesh(std::shared_ptr<MeshVertexData> vertices,
         std::vector<std::shared_ptr<Texture>> textures);
    void draw(const Shader& shader) const;

private:
    std::shared_ptr<MeshVertexData> vertices_;
    std::vector<std::shared_ptr<Texture>> textures_;
};

} // namespace rg

#endif // RG_RENDERER_MODEL_MESH_HPP
