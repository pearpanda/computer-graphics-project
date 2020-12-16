#ifndef RG_SPOTLIGHT_HPP
#define RG_SPOTLIGHT_HPP

#include <glm/vec3.hpp>
#include <rg/renderer/light/Light.hpp>
#include <rg/renderer/shader/Shader.hpp>

namespace rg {

class SpotLight : Light {
public:
    void apply(const Shader& shader, const std::string& fieldName) override;

private:
    glm::vec3 position_;
    glm::vec3 direction_;
    float cut_off_;
    float outer_cut_off_;

    float constant_;
    float linear_;
    float quadratic_;
};

} // namespace rg
#endif // RG_SPOTLIGHT_HPP
