#include <app/objects/Camera.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <spdlog/spdlog.h>

namespace app {

Camera::Camera(glm::vec3 position, glm::vec3 direction, float fov,
               float aspect_ratio, float near, float far) {
    auto angles = directionToYawPitch(glm::normalize(direction));
    yaw_ = angles.first;
    pitch_ = angles.second;
    transform_.scale = glm::vec3{1.0f};
    transform_.position = position;
    rotateBasis();

    view_.position = position;
    view_.vertical_fov = fov;
    view_.aspect_ratio = aspect_ratio;
    view_.z_near = near;
    view_.z_far = far;
}

std::pair<float, float> Camera::directionToYawPitch(const glm::vec3& spatial) {
    static constexpr auto pi = glm::pi<float>();

    float pitch = glm::asin(spatial.y);
    glm::vec2 planar{spatial.z, spatial.x};
    planar = glm::normalize(planar);

    float yaw = glm::acos(planar.x);
    if (planar.y < 0.0f)
        yaw = 2 * pi - yaw;
    return {yaw, pitch};
}

void Camera::move(const glm::vec3& delta) {
    transform_.position += delta;
    view_.position = transform_.position;
}

void Camera::rotate(float delta_yaw, float delta_pitch) {
    yaw_ += delta_yaw;
    pitch_ += delta_pitch;
    normalizeYawAndPitch();
    rotateBasis();
}

const rg::View& Camera::get_view() const {
    return view_;
}

glm::vec3 Camera::get_position() const {
    return transform_.position;
}
glm::vec3 Camera::get_direction() const {
    return transform_.get_forward_vector();
}
glm::vec3 Camera::get_up() const {
    return transform_.get_up_vector();
}
glm::vec3 Camera::get_right() const {
    return transform_.get_right_vector();
}

Camera& Camera::set_position(glm::vec3 position) {
    transform_.position = position;
    view_.position = transform_.position;
    return *this;
}

Camera& Camera::set_rotation(float yaw, float pitch) {
    yaw_ = yaw;
    pitch_ = pitch;
    normalizeYawAndPitch();
    rotateBasis();
    return *this;
}

Camera& Camera::set_direction(glm::vec3 direction) {
    auto angles = directionToYawPitch(glm::normalize(direction));
    yaw_ = angles.first;
    pitch_ = angles.second;
    rotateBasis();

    return *this;
}

void Camera::normalizeYawAndPitch() {
    static constexpr auto pi = glm::pi<float>();
    static constexpr auto pi_half = pi / 2.0f;

    // Normalize yaw and pitch
    // yaw in [0, 2*pi)
    yaw_ = glm::mod(yaw_, 2.0f * pi);
    // pitch in (-pi/2, pi/2)
    pitch_ = glm::clamp(pitch_, -pi_half + eps_, pi_half - eps_);
}

void Camera::rotateBasis() {
    transform_.orientation = glm::quat{glm::vec3{-pitch_, yaw_, 0.0f}};
    view_.direction = transform_.get_forward_vector();
    view_.up = transform_.get_up_vector();
}

} // namespace app
