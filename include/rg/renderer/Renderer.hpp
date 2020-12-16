#ifndef RG_RENDERER_HPP
#define RG_RENDERER_HPP

#include <GLFW/glfw3.h>
#include <rg/model/Ball.hpp>
#include <rg/renderer/camera/View.hpp>
#include <rg/renderer/model/Model.hpp>
#include <rg/renderer/shader/Shader.hpp>
#include <rg/renderer/shader/ShaderData.hpp>
#include <string>
#include <unordered_map>
#include <vector>

//#define DEBUG
// when DEBUG is defined, cursor isn't captured by the window

namespace rg {

class Renderer {
public:
    Renderer(Renderer& other) = delete;
    void operator=(const Renderer&) = delete;
    ~Renderer();

    void loop();

    class Builder;

private:
    Renderer(unsigned windowWidth, unsigned windowHeight, const char* title,
             View* camera, Ball* ball,
             std::unordered_map<std::string, std::string>& models,
             std::unordered_map<std::string, ShaderData*>& shaderData);
    void processInput(GLFWwindow* window);

    inline static Renderer* instance_ = nullptr;
    GLFWwindow* window_;
    View* camera_;
    Ball* ball_;
    std::unordered_map<Shader*, ShaderData*> shaders_;
    std::unordered_map<std::string, Model*> models_;

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

class Renderer::Builder {
public:
    Builder();
    Builder& set_window_width(unsigned windowWidth);
    Builder& set_window_height(unsigned windowHeight);
    Builder& set_window_title(const char* title);
    Builder& set_camera(View* camera);
    Builder& set_camera_position(glm::vec3 position);
    Builder& set_camera_direction(glm::vec3 direction);
    Builder& set_camera_up(glm::vec3 up);
    Builder& set_camera_horizontal_fov(float horizontal_fov);
    Builder& set_camera_vertical_fov(float vertical_fov);
    Builder& set_camera_z_near(float z_near);
    Builder& set_camera_z_far(float z_far);
    Builder& set_ball(Ball* ball);
    /**
     * Add a shader to be used with this renderer.
     * @param name name of the shader, without path or {.vs/.fs}.glsl
     * extension. Both shaders (vertex and fragment) must exist on
     * filesystem in res/shaders directory.
     * @return this
     */
    Builder& addShader(const std::string& name, ShaderData* data);
    /**
     * Add a model to be used with this renderer.
     * @param filepath filepath of the model, without res/objects prefix.
     * Model file must exist on filesystem at "res/objects/${filepath}".
     * @return this
     * @see Renderer::Builder::addModelToShader(unsigned, unsigned), to
     * associate the model with a shader.
     */
    Builder& addModel(const std::string& name, const std::string& filepath);

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
    std::unordered_map<std::string, std::string> models_;
    std::unordered_map<std::string, ShaderData*> shaders_;
};
} // namespace rg
#endif // RG_RENDERER_HPP
