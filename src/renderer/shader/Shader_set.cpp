#include <rg/renderer/shader/Shader.hpp>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace rg {

void Shader::set(const std::string& uniform, float value) const {
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void Shader::set(const std::string& uniform, int value) const {
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

void Shader::set(const std::string& uniform, const glm::vec2& value) const {
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniform2fv(location, 1, glm::value_ptr(value));
    }
}

void Shader::set(const std::string& uniform, const glm::vec3& value) const {
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniform3fv(location, 1, glm::value_ptr(value));
    }
}

void Shader::set(const std::string& uniform, const glm::vec4& value) const {
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniform4fv(location, 1, glm::value_ptr(value));
    }
}

void Shader::set(const std::string& uniform, const glm::mat2x2& value) const {
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}

void Shader::set(const std::string& uniform, const glm::mat3x3& value) const {
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}

void Shader::set(const std::string& uniform, const glm::mat4x4& value) const {
    int location = get_uniform_location(uniform);
    if (location != -1) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}

}; // namespace rg
