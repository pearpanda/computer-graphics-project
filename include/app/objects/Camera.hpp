#ifndef APP_OBJECTS_CAMERA_HPP
#define APP_OBJECTS_CAMERA_HPP

#include <rg/renderer/camera/View.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace app {

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 direction, float fov,
           float aspect_ratio, float near, float far);

    const rg::View& get_view() const;
    [[nodiscard]] glm::vec3 get_position() const;
    [[nodiscard]] glm::vec3 get_direction() const;
    [[nodiscard]] glm::vec3 get_up() const;
    [[nodiscard]] glm::vec3 get_right() const;

    Camera& set_position(glm::vec3 position);
    Camera& set_rotation(float yaw, float pitch);
    Camera& set_direction(glm::vec3 direction);
    void move(const glm::vec3& delta);
    void rotate(float delta_yaw, float delta_pitch);

private:
    float yaw_, pitch_;
    rg::View view_;
    static constexpr float eps_ = 1e-3;

    glm::vec3 position_{};
    glm::vec3 direction_{};
    glm::vec3 up_{};
    glm::vec3 right_{};

    static std::pair<float, float> directionToYawPitch(const glm::vec3& vec);
    void rotateBasis();
    void normalizeYawAndPitch();
};

} // namespace app

#endif // RG_RENDERER_CAMERA_CAMERA_HPP
