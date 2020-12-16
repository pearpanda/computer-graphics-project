#include <rg/renderer/shader/ShaderData.hpp>
#include <spdlog/spdlog.h>

namespace rg {
std::unordered_map<std::string, ShaderData::ModelData*>&
ShaderData::get_models() {
    return model_data_;
}
std::vector<Light*>& ShaderData::get_lights() {
    return lights_;
}

ShaderData::Builder& ShaderData::Builder::addModel(const std::string& name) {
    last_model_name_ = name;
    instance->model_data_.insert(
            std::make_pair(name, new ShaderData::ModelData{}));
    return *this;
}

ShaderData::~ShaderData() {
    for (const auto& model : model_data_) {
        delete model.second;
    }
    for (const auto& light : lights_) {
        delete light;
    }
}

ShaderData::Builder&
ShaderData::Builder::set_model_translate(const glm::vec3& translate) {
    if (last_model_name_.empty())
        throw std::runtime_error{"No model inserted!"};

    instance->model_data_[last_model_name_]->translate_ = translate;
    return *this;
}
ShaderData::Builder&
ShaderData::Builder::set_model_scale(const glm::vec3& scale) {
    if (last_model_name_.empty())
        throw std::runtime_error{"No model inserted!"};

    instance->model_data_[last_model_name_]->scale_ = scale;
    return *this;
}
ShaderData::Builder& ShaderData::Builder::set_shininess(const float shininess) {
    if (last_model_name_.empty())
        throw std::runtime_error{"No model inserted!"};

    instance->model_data_[last_model_name_]->shininess_ = shininess;
    return *this;
}
ShaderData::Builder& ShaderData::Builder::addLight(Light* light) {
    instance->lights_.push_back(light);
    return *this;
}
ShaderData* ShaderData::Builder::build() {
    if (instance->lights_.empty()) {
        spdlog::warn(
                "No lights are set! Models will be rendered completely dark.");
    }
    return instance;
}
} // namespace rg