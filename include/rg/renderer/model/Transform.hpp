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

    [[nodiscard]] glm::vec3 get_forward_vector() const;
    [[nodiscard]] glm::vec3 get_backward_vector() const;
    [[nodiscard]] glm::vec3 get_up_vector() const;
    [[nodiscard]] glm::vec3 get_down_vector() const;
    [[nodiscard]] glm::vec3 get_left_vector() const;
    [[nodiscard]] glm::vec3 get_right_vector() const;

private:
    // Canonical basis
    static constexpr glm::vec3 LEFT{1.0f, 0.0f, 0.0f};
    static constexpr glm::vec3 UP{0.0f, 1.0f, 0.0f};
    static constexpr glm::vec3 FORWARD{0.0f, 0.0f, 1.0f};

    // Extensions
    static constexpr glm::vec3 RIGHT = -LEFT;
    static constexpr glm::vec3 DOWN = -UP;
    static constexpr glm::vec3 BACKWARD = -FORWARD;
};

} // namespace rg

#endif // RG_RENDERER_MODEL_TRANSFORM_HPP
