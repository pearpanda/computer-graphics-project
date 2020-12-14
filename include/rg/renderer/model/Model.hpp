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

    void set_translate_vector(glm::vec3 translate);
    void set_scale_vector(glm::vec3 scale);
    glm::vec3 get_translate_vector();
    glm::vec3 get_scale_vector();

private:
    std::vector<Mesh> meshes_;
    glm::vec3 translate_ = glm::vec3{0.0f, 0.0f, -3.0f};
    glm::vec3 scale_ = glm::vec3{1.0f, 1.0f, 1.0f};
};

} // namespace rg

#endif // RG_RENDERER_MODEL_MODEL_HPP
