#include <rg/renderer/shader/Shader.hpp>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

namespace rg {

namespace {

unsigned int compileShader(unsigned int type, const std::string& source) {
    const char* c_string_source = source.c_str();
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &c_string_source, nullptr);
    glCompileShader(id);

    int success, required_size;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &required_size);
        std::vector<char> tmp(required_size);
        glGetShaderInfoLog(id, required_size, nullptr, tmp.data());
        std::string error(tmp.begin(), tmp.end());

        spdlog::error("RG::SHADER::COMPILE_SHADER: {}", error);
        return 0;
    }

    return id;
}

unsigned int linkProgram(unsigned int vs, unsigned int fs) {
    unsigned int id = glCreateProgram();
    glAttachShader(id, vs);
    glAttachShader(id, fs);
    glLinkProgram(id);

    int success, required_size;
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &required_size);
        std::vector<char> tmp(required_size);
        glGetProgramInfoLog(id, required_size, nullptr, tmp.data());
        std::string error(tmp.begin(), tmp.end());

        spdlog::error("RG::SHADER::LINK_PROGRAM: {}", error);
    }

    return id;
}

} // namespace

Shader Shader::compile(const std::string& vertexSource,
                       const std::string& fragmentSource) {
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    unsigned int program = linkProgram(vs, fs);

    glUseProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return Shader{program};
}

int Shader::get_uniform_location(const std::string& name) const {
    int location = glGetUniformLocation(shader_id_, name.c_str());
    if (location == -1) {
        spdlog::warn("RG::SHADER::GET_LOCATION: location for uniform {} is "
                     "non-existent",
                     name);
    }
    return location;
}

void Shader::bind() const {
    glUseProgram(shader_id_);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void Shader::unbind() const {
    glUseProgram(0);
}

Shader::Shader(unsigned int id) : shader_id_{id} {
}

Shader::~Shader() {
    glDeleteProgram(shader_id_);
}

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

} // namespace rg
