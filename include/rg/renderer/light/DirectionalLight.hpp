#ifndef RG_DIRECTIONALLIGHT_HPP
#define RG_DIRECTIONALLIGHT_HPP

#include <glm/vec3.hpp>
#include <rg/renderer/light/Light.hpp>
#include <rg/renderer/shader/Shader.hpp>

namespace rg {

class DirectionalLight : Light {
public:
    void apply(const Shader& shader, const std::string& fieldName) override;

private:
    glm::vec3 direction_;
};

} // namespace rg
#endif // RG_DIRECTIONALLIGHT_HPP
