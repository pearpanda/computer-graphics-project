#include <rg/renderer/shader/Shader.hpp>

#include <glad/glad.h>

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
    glGetProgramiv(id, GL_COMPILE_STATUS, &success);
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
    if (location == -1)
        spdlog::warn("RG::SHADER::GET_LOCATION: location for uniform {} is "
                     "non-existant",
                     name);
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

} // namespace rg
