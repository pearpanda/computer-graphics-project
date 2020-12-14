#ifndef RG_RENDERER_HPP
#define RG_RENDERER_HPP

#include <GLFW/glfw3.h>
#include <rg/model/Ball.hpp>
#include <rg/renderer/camera/View.hpp>
#include <rg/renderer/model/Model.hpp>
#include <rg/renderer/shader/Shader.hpp>
#include <string>
#include <unordered_map>
#include <vector>

//#define DEBUG
// when DEBUG is defined, cursor isn't captured by the window

namespace rg {

class Renderer {
private:
    struct ModelData {
        std::string filepath;
        glm::vec3 scale = glm::vec3{1.0f, 1.0f, 1.0f};
        glm::vec3 translate = glm::vec3{0.0f, 0.0f, -3.0f};
    };

public:
    Renderer(Renderer& other) = delete;
    void operator=(const Renderer&) = delete;

    ~Renderer();
    void loop();

    class Builder {
    public:
        Builder();
        Builder* set_window_width(unsigned windowWidth);
        Builder* set_window_height(unsigned windowHeight);
        Builder* set_window_title(const char* title);
        Builder* set_camera(View* camera);
        Builder* set_camera_position(glm::vec3 position);
        Builder* set_camera_direction(glm::vec3 direction);
        Builder* set_camera_up(glm::vec3 up);
        Builder* set_camera_horizontal_fov(float horizontal_fov);
        Builder* set_camera_vertical_fov(float vertical_fov);
        Builder* set_camera_z_near(float z_near);
        Builder* set_camera_z_far(float z_far);
        Builder* set_ball(Ball* ball);
        /**
         * Add a shader to be used with this renderer.
         * @param name name of the shader, without path or {.vs/.fs}.glsl
         * extension. Both shaders (vertex and fragment) must exist on
         * filesystem in res/shaders directory.
         * @return this
         */
        Builder* addShader(const std::string& name);
        /**
         * Add a model to be used with this renderer.
         * @param filepath filepath of the model, without res/objects prefix.
         * Model file must exist on filesystem at "res/objects/${filepath}".
         * @return this
         * @see Renderer::Builder::addModelToShader(unsigned, unsigned), to
         * associate the model with a shader.
         */
        Builder* addModel(const std::string& filepath);
        /**
         * Add a model to shader. This passes model with the given index to the
         * shader with the given index as a uniform with name
         * "model" + modelIndex (i.e. "model0", "model1", etc). Zeroth shader
         * is the one for which addShader(std::string) has been called first,
         * etc. Same logic applies to models.
         * @param shaderIndex index of the shader
         * @param modelIndex index of the model
         * @return this
         */
        Builder* set_model_scale(glm::vec3 scale);
        Builder* set_model_translate(glm::vec3 translate);
        // this is not the prettiest way, but for small numbers of
        // shaders/models it'll work. Maybe use addShader method for this, or
        // add this info to Shader class.
        Builder* addModelToShader(unsigned shaderIndex, unsigned modelIndex);

        Renderer* build();

    private:
        unsigned window_height_ = 800;
        unsigned window_width_ = 600;
        const char* title_ = "Window";

        View* camera_ = new rg::View{glm::vec3{0.0f, 0.0f, -3.0f},
                                     glm::vec3{0.0f, 0.0f, 1.0f},
                                     glm::vec3{0.0f, 1.0f, 0.0f},
                                     60.0f,
                                     static_cast<float>(window_width_) /
                                             window_height_ * 60.0f,
                                     0.1f,
                                     100.0f};
        Ball* ball_ = new Ball(Court(), 0, 0, 2);
        std::vector<std::string> shader_names_;
        std::vector<ModelData> model_files_;
        std::unordered_map<unsigned, std::vector<unsigned>> model_mappings_;
    };

private:
    Renderer(unsigned windowWidth, unsigned windowHeight, const char* title,
             View* camera, Ball* ball,
             const std::vector<std::string>& shaderNames,
             const std::vector<ModelData>& modelData,
             std::unordered_map<unsigned, std::vector<unsigned>> modelMappings);
    void processInput(GLFWwindow* window);

    inline static Renderer* instance_ = nullptr;
    GLFWwindow* window_;
    View* camera_;
    Ball* ball_;
    std::vector<Shader*> shaders_;
    std::vector<Model*> models_;
    std::unordered_map<unsigned, std::vector<unsigned>> model_mappings_;

    float last_x_;
    float last_y_;
    bool first_mouse_ = true;
    float sensitivity_ = 0.003f;
    float yaw_ = 0.0f, pitch_ = 0.0f;
    // timing
    float delta_time_ = 0.0f;
    float last_frame_ = 0.0f;
    struct Callbacks;
};
} // namespace rg
#endif // RG_RENDERER_HPP
