#ifndef RG_RENDERER_MODEL_TRANSFORM_HPP
#define RG_RENDERER_MODEL_TRANSFORM_HPP

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>

namespace rg {

struct Transform {
    glm::vec3 position{0.0f};
    glm::quat orientation{glm::vec3{0.0f, 0.0f, 0.0f}};
    glm::vec3 scale{1.0f};

    [[nodiscard]] glm::mat4 get_model_matrix() const;
    [[nodiscard]] glm::mat4 get_normal_matrix() const;
    [[nodiscard]] glm::mat4 get_translate_matrix() const;
    [[nodiscard]] glm::mat4 get_rotate_matrix() const;
    [[nodiscard]] glm::mat4 get_scale_matrix() const;
};

} // namespace rg

#endif // RG_RENDERER_MODEL_TRANSFORM_HPP
