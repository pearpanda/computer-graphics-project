#ifndef RG_POINTLIGHT_HPP
#define RG_POINTLIGHT_HPP

#include <glm/vec3.hpp>
#include <rg/renderer/light/Light.hpp>
#include <rg/renderer/shader/Shader.hpp>

namespace rg {

class PointLight : Light {
public:
    void apply(const Shader& shader, const std::string& fieldName) override;

private:
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;
};

} // namespace rg
#endif // RG_POINTLIGHT_HPP
