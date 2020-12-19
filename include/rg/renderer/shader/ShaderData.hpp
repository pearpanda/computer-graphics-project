#ifndef RG_SHADERDATA_HPP
#define RG_SHADERDATA_HPP

#include <glm/vec3.hpp>
#include <rg/renderer/light/Light.hpp>
#include <unordered_map>
#include <vector>

namespace rg {

/**
 * Sidecar object for Shader objects. It contains data shader might need (e.g.
 * which models to draw, where are the lights), but doesn't tie it to the Shader
 * object itself.
 *
 * This makes sense because we're treating Shader as a heavyweight object
 * (initializing it requires GLSL compilation) and this holds lightweight data
 * which can be attached to any shader.
 */
// yeah, this docstring describes "why" instead of "how", but in this case "how"
// is fairly obvious and "how to use" might not be
class ShaderData {
public:
    class Builder;
    /**
     * Data specific to a Model. Model is also a heavyweight, and this is its
     * sidecar. To avoid overloading the API, we're treating this basically
     * as an implementation detail (it needs to be public so it can be consumed
     * by Shader, but it's set by the same Builder).
     */
    struct ModelData {
        // this doesn't have dedicated builder, so we specify defaults here
        glm::vec3 translate_ = glm::vec3{0.0f, 0.0f, -3.0f};
        glm::vec3 scale_ = glm::vec3{1.0f, 1.0f, 1.0f};
        float shininess_ = 64.0f;
    };
    [[nodiscard]] std::unordered_map<std::string, ModelData*>& get_models();
    [[nodiscard]] std::vector<Light*>& get_lights();

    ~ShaderData();

private:
    std::unordered_map<std::string, ModelData*> model_data_;
    std::vector<Light*> lights_;

    friend class ShaderDataBuilder;
};

/**
 * Builds a ShaderData instance, along with ModelData where needed.
 *
 */
// Neither ShaderData nor ModelData are extraordinarily complex, but this
// Builder provides a single interface to build everything in one statement.
class ShaderData::Builder {
public:
    /**
     * Add a model to this shader. All we need is a name of the object.
     * Model needs to be previously defined and associated with the context
     * (@see rg::Initializer:addModel). This is to prevent tying a heavyweight
     * model with a lightweight ShaderData.
     *
     * @param name name (reference) of the model
     * @return *this
     */
    ShaderData::Builder& addModel(const std::string& name);
    // these methods set the properties of the last added model
    // in this way, we've flattened what could've been two builders
    ShaderData::Builder& set_model_translate(const glm::vec3& translate);
    ShaderData::Builder& set_model_scale(const glm::vec3& scale);
    ShaderData::Builder& set_model_shininess(float shininess);
    /**
     * Add a light to the scene. Use the Light's Builder to make a valid object.
     * @param light light which will illuminate the scene.
     * @return *this
     */
    ShaderData::Builder& addLight(Light* light);
    ShaderData* build();

private:
    // we don't need to store bunch of properties here, but rather construct
    // instance right away and set its fields where needed. This builder
    // basically provides a flattened "chained setters" interface.
    ShaderData* instance = new ShaderData();
    // to use set_model_ methods we need to know which one was added last
    std::string last_model_name_;
};

} // namespace rg
#endif // RG_SHADERDATA_HPP
