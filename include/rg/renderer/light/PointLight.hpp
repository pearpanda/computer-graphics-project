#ifndef RG_POINTLIGHT_HPP
#define RG_POINTLIGHT_HPP

#include <glm/vec3.hpp>
#include <rg/renderer/light/Light.hpp>
#include <rg/renderer/shader/Shader.hpp>

namespace rg {

class PointLight : Light {
public:
    void apply(Shader* shader, const std::string& fieldName) const override;
    [[nodiscard]] inline std::string getShaderFieldName() const override;

    ~PointLight() override = default;

private:
    glm::vec3 position_;

    float constant_;
    float linear_;
    float quadratic_;
};

} // namespace rg
#endif // RG_POINTLIGHT_HPP
