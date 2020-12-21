#include <rg/renderer/camera/Camera.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <spdlog/spdlog.h>

namespace rg {

Camera::Camera(glm::vec3 position, glm::vec3 direction, float fov,
               float aspect_ratio, float near, float far)
        : position_{position} {
    auto angles = directionToYawPitch(glm::normalize(direction));
    yaw_ = angles.first;
    pitch_ = angles.second;
    // direction_, up_, right_
    rotateBasis();

    view_.position = position;
    view_.vertical_fov = fov;
    view_.aspect_ratio = aspect_ratio;
    view_.z_near = near;
    view_.z_far = far;
    view_.direction = direction_;
    view_.up = up_;
}

std::pair<float, float> Camera::directionToYawPitch(const glm::vec3& vec) {
    static constexpr auto pi = glm::pi<float>();

    float pitch = glm::asin(vec.y);
    float yaw = glm::acos(-vec.z);
    if (vec.x > 0.0f)
        yaw = 2 * pi - yaw;
    return {yaw, pitch};
}

void Camera::rotateBasis() {
    static constexpr glm::vec3 front{0.0f, 0.0f, -1.0f};
    static constexpr glm::vec3 up{0.0f, 1.0f, 0.0f};
    static constexpr glm::vec3 right{1.0f, 0.0f, 0.0f};

    auto q = glm::quat(glm::vec3{pitch_, yaw_, 0.0f});
    direction_ = q * front;
    up_ = q * up;
    right_ = q * right;

    view_.direction = direction_;
    view_.up = up_;
}

void Camera::move(const glm::vec3& delta) {
    position_ += delta;
    view_.position = position_;
}

void Camera::rotate(float delta_yaw, float delta_pitch) {
    yaw_ += delta_yaw;
    pitch_ += delta_pitch;
    normalizeYawAndPitch();
    rotateBasis();
}

const View& Camera::get_view() const {
    return view_;
}

glm::vec3 Camera::get_position() const {
    return position_;
}
glm::vec3 Camera::get_direction() const {
    return direction_;
}
glm::vec3 Camera::get_up() const {
    return up_;
}
glm::vec3 Camera::get_right() const {
    return right_;
}
Camera& Camera::set_position(glm::vec3 position) {
    position_ += position;
    view_.position = position_;
    return *this;
}
Camera& Camera::set_rotation(float yaw, float pitch) {
    yaw_ = yaw;
    pitch_ = pitch;
    normalizeYawAndPitch();
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

} // namespace rg
