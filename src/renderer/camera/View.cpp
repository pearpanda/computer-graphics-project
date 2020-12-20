#include <rg/renderer/camera/View.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace rg {

View::View()
        : position{0.0f}, direction{0.0f, 0.0f, -1.0f}, up{0.0f, 1.0f, 0.0f},
          vertical_fov{60.0f},
          aspect_ratio{4.0f / 3.0f}, z_near{0.1f}, z_far{100.0f} {
}

View::View(glm::vec3 position, glm::vec3 direction, glm::vec3 up,
           float vertical_fov, float aspect_ratio, float z_near, float z_far)
        : position{position}, direction{direction}, up{up},
          vertical_fov{vertical_fov},
          aspect_ratio{aspect_ratio}, z_near{z_near}, z_far{z_far} {
}

glm::mat4 View::get_view_matrix() const {
    return glm::lookAt(position, position + direction, up);
}

glm::mat4 View::get_projection_matrix() const {
    return glm::perspective(glm::radians(vertical_fov), aspect_ratio, z_near,
                            z_far);
}

} // namespace rg
