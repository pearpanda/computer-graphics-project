#ifndef RG_RENDERER_CAMERA_VIEW_HPP
#define RG_RENDERER_CAMERA_VIEW_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace rg {

struct View {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    float horizontal_fov;
    float vertical_fov;
    float z_near;
    float z_far;

    View(glm::vec3 position, glm::vec3 direction, glm::vec3 up,
         float horizontal_fov, float vertical_fov, float z_near, float z_far);

    [[nodiscard]] glm::mat4 get_view_matrix() const;
    [[nodiscard]] glm::mat4 get_projection_matrix() const;
};

} // namespace rg

#endif // RG_RENDERER_CAMERA_VIEW_HPP
