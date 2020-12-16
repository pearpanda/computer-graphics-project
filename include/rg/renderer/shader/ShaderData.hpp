#ifndef RG_SHADERDATA_HPP
#define RG_SHADERDATA_HPP

#include <glm/vec3.hpp>
#include <rg/renderer/light/Light.hpp>
#include <unordered_map>
#include <vector>

namespace rg {

class ShaderData {
public:
    class Builder;
    struct ModelData {
        glm::vec3 translate_ = glm::vec3{0.0f, 0.0f, -3.0f};
        glm::vec3 scale_ = glm::vec3{1.0f, 1.0f, 1.0f};
        float shininess_ = 1.0f;
    };
    [[nodiscard]] std::unordered_map<std::string, ModelData*>& get_models();
    [[nodiscard]] std::vector<Light*>& get_lights();

    ~ShaderData();

private:
    std::unordered_map<std::string, ModelData*> model_data_;
    std::vector<Light*> lights_;

    friend class ShaderDataBuilder;
};

class ShaderData::Builder {
public:
    ShaderData::Builder& addModel(const std::string& name);
    ShaderData::Builder& set_model_translate(const glm::vec3& translate);
    ShaderData::Builder& set_model_scale(const glm::vec3& scale);
    ShaderData::Builder& set_shininess(float shininess);
    ShaderData::Builder& addLight(Light* light);
    ShaderData* build();

private:
    ShaderData* instance = new ShaderData();
    std::string last_model_name_;
};

} // namespace rg
#endif // RG_SHADERDATA_HPP
