#ifndef RG_LIGHT_HPP
#define RG_LIGHT_HPP

#include <glm/vec3.hpp>
#include <rg/renderer/shader/Shader.hpp>

namespace rg {

/**
 * Base class for all lights. Assumes Phong lighting: all lights consitute of
 * ambient, diffuse and specular components.
 */
class Light {
public:
    /**
     * Apply light to a shader. Sets appropriate structures in the shader.
     * @param shader shader name
     * @param fieldName name of the field whose value should be set,
     * e.g. spotLight[1]
     */
    virtual void apply(Shader* shader, const std::string& fieldName) const;
    /**
     * Name of the field which represents this type of lighting in the shader,
     * e.g. spotLights. We assume shaders consist of arrays of lights; brackets
     * and index are appended to this name to actually access the field.
     * @return name of this type of lighting in the shader.
     */
    // This is a base class, not a type of light, so it's a pure virtual method
    [[nodiscard]] virtual std::string getShaderFieldName() const = 0;

    virtual ~Light() = default;

    class Builder;

protected:
    Light(glm::vec3& ambient, glm::vec3& diffuse, glm::vec3& specular);

private:
    glm::vec3 ambient_;
    glm::vec3 diffuse_;
    glm::vec3 specular_;
};

/**
 * Builder for constructing light objects. Constructing an object which contains
 * only three fields is not a problem per se, but different types of lighting
 * have more complicated structures and using a constructor is unwieldy.
 * This is a base class—it cannot actually build anything (build() method is
 * pure virtual).
 *
 * Builder should predefine all non-required parameters, so it can be called
 * like Builder().build() and produce something sensible. If that's not
 * possible, all required parameters should go to builder's constructor (make
 * this as minimal as possible).
 */
class Light::Builder {
public:
    // these are manipulating private fields, so they are not virtual
    Light::Builder& set_ambient(const glm::vec3& ambient);
    Light::Builder& set_diffuse(const glm::vec3& diffuse);
    Light::Builder& set_specular(const glm::vec3& specular);

    virtual Light* build() = 0;

protected:
    glm::vec3 b_ambient_ = glm::vec3{0.1f, 0.1f, 0.1f};
    glm::vec3 b_diffuse_ = glm::vec3{0.2f, 0.2f, 0.2f};
    glm::vec3 b_specular_ = glm::vec3{1.0f, 1.0f, 1.0f};
};
} // namespace rg
#endif // RG_LIGHT_HPP
