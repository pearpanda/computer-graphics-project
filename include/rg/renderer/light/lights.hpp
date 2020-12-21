#ifndef RG_RENDERER_LIGHT_LIGHTS_HPP
#define RG_RENDERER_LIGHT_LIGHTS_HPP

#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

namespace rg {

struct LightColor {
    glm::vec3 ambient{0.1f};
    glm::vec3 diffuse{1.0f};
    glm::vec3 specular{1.0f};
};

// See https://learnopengl.com/Lighting/Light-casters
// Section: "Choosing the right values"
struct LightAttenuation {
    float constant{1.0f};
    float linear{0.09f};
    float quadratic{0.032f};
};

struct DirectionalLight {
    glm::vec3 direction{0.0f, 0.0f, -1.0f};
    LightColor color;
};

struct PointLight {
    glm::vec3 position{0.0f};
    LightAttenuation attenuation;
    LightColor color;
};

struct SpotLight {
    glm::vec3 position{0.0f};
    glm::vec3 direction{0.0f, 0.0f, -1.0f};

    // The angles are stored as cosines, rather than the actual angles
    float cutoff_angle{glm::cos(glm::radians(80.0f))};
    float weaken_angle{glm::cos(glm::radians(60.0f))};

    LightAttenuation attenuation;
    LightColor color;
};

} // namespace rg

#endif // RG_RENDERER_LIGHT_LIGHTS_HPP
