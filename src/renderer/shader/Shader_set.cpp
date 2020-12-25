#include <rg/renderer/shader/Shader.hpp>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

namespace rg {

void Shader::set(const std::string& uniform, const glm::vec2& value) const {
    bind();
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniform2fv(location, 1, glm::value_ptr(value));
    }
}

void Shader::set(const std::string& uniform, const glm::vec3& value) const {
    bind();
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniform3fv(location, 1, glm::value_ptr(value));
    }
}

void Shader::set(const std::string& uniform, const glm::vec4& value) const {
    bind();
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniform4fv(location, 1, glm::value_ptr(value));
    }
}

void Shader::set(const std::string& uniform, const glm::mat2x2& value) const {
    bind();
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}

void Shader::set(const std::string& uniform, const glm::mat3x3& value) const {
    bind();
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}

void Shader::set(const std::string& uniform, const glm::mat4x4& value) const {
    bind();
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}

void Shader::set(const Transform& transform) const {
    bind();
    this->set("model_matrix", transform.get_model_matrix());
    this->set("normal_matrix", transform.get_normal_matrix());
}

void Shader::set(const View& view) const {
    bind();
    this->set("view_matrix", view.get_view_matrix());
    this->set("projection_matrix", view.get_projection_matrix());
}

void Shader::set_float(const std::string& uniform, float value) const {
    bind();
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void Shader::set_int(const std::string& uniform, int value) const {
    bind();
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

void Shader::set(const std::string& uniform, const LightColor& color) const {
    this->set(uniform + ".ambient", color.ambient);
    this->set(uniform + ".diffuse", color.diffuse);
    this->set(uniform + ".specular", color.specular);
}

void Shader::set(const std::string& uniform,
                 const LightAttenuation& attenuation) const {
    this->set_float(uniform + ".constant", attenuation.constant);
    this->set_float(uniform + ".linear", attenuation.linear);
    this->set_float(uniform + ".quadratic", attenuation.quadratic);
}

void Shader::set(const std::string& uniform,
                 const DirectionalLight& light) const {
    bind();
    this->set(uniform + ".direction", light.direction);
    this->set(uniform + ".color", light.color);
}

void Shader::set(const std::string& uniform, const PointLight& light) const {
    bind();
    this->set(uniform + ".position", light.position);
    this->set(uniform + ".color", light.color);
    this->set(uniform + ".attenuation", light.attenuation);
}

void Shader::set(const std::string& uniform, const SpotLight& light) const {
    bind();
    this->set(uniform + ".position", light.position);
    this->set(uniform + ".direction", light.direction);
    this->set(uniform + ".color", light.color);
    this->set(uniform + ".attenuation", light.attenuation);
    this->set_float(uniform + ".cutoff_angle", light.cutoff_angle);
    this->set_float(uniform + ".weaken_angle", light.weaken_angle);
}

}; // namespace rg
