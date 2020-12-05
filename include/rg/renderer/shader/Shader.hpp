#ifndef RG_RENDERER_SHADER_SHADER_HPP
#define RG_RENDERER_SHADER_SHADER_HPP

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <string>

namespace rg {

class Shader {
public:
    static Shader compile(const std::string& vertex_source,
                          const std::string& fragment_source);
    ~Shader();
    void bind() const;
    void unbind() const;

    void set(const std::string& uniform, float value);
    void set(const std::string& uniform, const glm::vec2& value);
    void set(const std::string& uniform, const glm::vec3& value);
    void set(const std::string& uniform, const glm::vec4& value);
    void set(const std::string& uniform, const glm::mat2x2& value);
    void set(const std::string& uniform, const glm::mat3x3& value);
    void set(const std::string& uniform, const glm::mat4x4& value);

private:
    // NOLINTNEXTLINE(google-explicit-constructor)
    Shader(unsigned int id);
    [[nodiscard]] int get_uniform_location(const std::string& name) const;
    unsigned int shader_id_;
};

} // namespace rg

#endif // RG_RENDERER_SHADER_SHADER_HPP
