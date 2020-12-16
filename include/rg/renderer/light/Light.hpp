#ifndef RG_LIGHT_HPP
#define RG_LIGHT_HPP

#include <glm/vec3.hpp>
#include <rg/renderer/shader/Shader.hpp>

namespace rg {

class Light {
public:
    virtual void apply(const Shader& shader, const std::string& fieldName);
    // todo figure out constructor (I'd really like to avoid setting all
    // SpotLight fields at once)

private:
    glm::vec3 ambient_;
    glm::vec3 diffuse_;
    glm::vec3 specular_;
};

} // namespace rg
#endif // RG_LIGHT_HPP
