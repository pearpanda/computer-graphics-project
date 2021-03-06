#ifndef RG_RENDERER_SHADER_SHADER_HPP
#define RG_RENDERER_SHADER_SHADER_HPP

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <rg/renderer/camera/View.hpp>
#include <rg/renderer/light/lights.hpp>
#include <rg/renderer/model/Transform.hpp>

#include <string>

namespace rg {

class Shader {
public:
    static Shader compile(const std::string& vertex_source,
                          const std::string& fragment_source);
    ~Shader();
    void bind() const;
    void unbind() const;

    void set(const std::string& uniform, const glm::vec2& value) const;
    void set(const std::string& uniform, const glm::vec3& value) const;
    void set(const std::string& uniform, const glm::vec4& value) const;
    void set(const std::string& uniform, const glm::mat2x2& value) const;
    void set(const std::string& uniform, const glm::mat3x3& value) const;
    void set(const std::string& uniform, const glm::mat4x4& value) const;
    void set(const rg::Transform& transform) const;
    void set(const rg::View& view) const;
    void set(const std::string& uniform,
             const rg::DirectionalLight& light) const;
    void set(const std::string& uniform, const rg::PointLight& light) const;
    void set(const std::string& uniform, const rg::SpotLight& light) const;

    void set_int(const std::string& uniform, int value) const;
    void set_float(const std::string& uniform, float value) const;

private:
    // NOLINTNEXTLINE(google-explicit-constructor)
    Shader(unsigned int id);
    [[nodiscard]] int get_uniform_location(const std::string& name) const;
    unsigned int shader_id_;

    void set(const std::string& uniform, const rg::LightColor& color) const;
    void set(const std::string& uniform,
             const rg::LightAttenuation& attenuation) const;
};

} // namespace rg

#endif // RG_RENDERER_SHADER_SHADER_HPP
