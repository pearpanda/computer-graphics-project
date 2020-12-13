#include <rg/renderer/camera/View.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace rg {

View::View(glm::vec3 position, glm::vec3 direction, glm::vec3 up,
           float horizontal_fov, float vertical_fov, float z_near, float z_far)
        : position{position}, direction{direction}, up{up},
          horizontal_fov{horizontal_fov},
          vertical_fov{vertical_fov}, z_near{z_near}, z_far{z_far} {
}

glm::mat4 View::get_view_matrix() const {
    return glm::lookAt(position, position + direction, up);
}

glm::mat4 View::get_projection_matrix() const {
    return glm::perspective(glm::radians(horizontal_fov),
                            glm::radians(vertical_fov), z_near, z_far);
}

} // namespace rg
