#ifndef RG_UTIL_COMMON_MESHES_HPP
#define RG_UTIL_COMMON_MESHES_HPP

#include <rg/renderer/buffer/IndexBuffer.hpp>
#include <rg/renderer/buffer/VertexArray.hpp>
#include <rg/renderer/buffer/VertexBuffer.hpp>
#include <rg/renderer/model/Mesh.hpp>
#include <rg/util/layouts.hpp>

#include <memory>

namespace rg::util {

std::shared_ptr<MeshVertexData> surfaceQuad();
std::shared_ptr<MeshVertexData> fullCube();

} // namespace rg::util

#endif // RG_UTIL_COMMON_MESHES_HPP
