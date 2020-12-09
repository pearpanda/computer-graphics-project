#ifndef RG_RENDERER_MODEL_MESH_HPP
#define RG_RENDERER_MODEL_MESH_HPP

#include <rg/renderer/buffer/IndexBuffer.hpp>
#include <rg/renderer/buffer/VertexArray.hpp>
#include <rg/renderer/buffer/VertexBuffer.hpp>
#include <rg/renderer/model/Texture.hpp>
#include <rg/renderer/shader/Shader.hpp>

#include <memory>

namespace rg {

class Mesh {
public:
    Mesh(std::shared_ptr<VertexArray> va, std::shared_ptr<VertexBuffer> vb,
         std::shared_ptr<IndexBuffer> ib,
         std::vector<std::shared_ptr<Texture>> textures);
    void draw(const Shader& shader) const;

private:
    std::shared_ptr<VertexArray> va_;
    std::shared_ptr<VertexBuffer> vb_;
    std::shared_ptr<IndexBuffer> ib_;
    std::vector<std::shared_ptr<Texture>> textures_;
};

} // namespace rg

#endif // RG_RENDERER_MODEL_MESH_HPP
