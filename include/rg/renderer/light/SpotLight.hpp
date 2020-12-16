#ifndef RG_SPOTLIGHT_HPP
#define RG_SPOTLIGHT_HPP

#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <rg/renderer/light/Light.hpp>
#include <rg/renderer/shader/Shader.hpp>

namespace rg {

class SpotLight : public PointLight {
    // SpotLight is also a DirectionalLight, but using multiple inheritance for
    // a single field seems like more trouble than benefit
public:
    void apply(Shader* shader, const std::string& fieldName) const override;
    [[nodiscard]] inline std::string getShaderFieldName() const override;

    ~SpotLight() override = default;

    class Builder;

protected:
    SpotLight(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
              glm::vec3 position, glm::vec3 direction, float cutOff,
              float outerCutOff, float constant, float linear, float quadratic);

private:
    glm::vec3 direction_;
    float cut_off_;
    float outer_cut_off_;
};

class SpotLight::Builder : public PointLight::Builder {

public:
    Builder(const glm::vec3& position, const glm::vec3& direction);

    SpotLight::Builder& set_cut_off(float cutOff);
    SpotLight::Builder& set_outer_cut_off(float outerCutOff);

    SpotLight* build() override;

protected:
    const glm::vec3 direction_;
    float cut_off_ = glm::cos(glm::radians(12.5f));
    float outer_cut_off_ = glm::cos(glm::radians(17.5f));
};

} // namespace rg
#endif // RG_SPOTLIGHT_HPP
