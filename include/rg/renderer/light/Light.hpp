#ifndef RG_LIGHT_HPP
#define RG_LIGHT_HPP

#include <glm/vec3.hpp>
#include <rg/renderer/shader/Shader.hpp>

namespace rg {

class Light {
public:
    virtual void apply(Shader* shader, const std::string& fieldName) const;
    [[nodiscard]] virtual std::string getShaderFieldName() const = 0;
    // todo figure out constructor (I'd really like to avoid setting all
    //  SpotLight fields at once)

    virtual ~Light() = default;

private:
    glm::vec3 ambient_ = glm::vec3{0.2f, 0.2f, 0.1f};
    glm::vec3 diffuse_ = glm::vec3{0.3f, 0.25f, 0.3f};
    glm::vec3 specular_ = glm::vec3{1.0f, 1.0f, 1.0f};
};

} // namespace rg
#endif // RG_LIGHT_HPP
