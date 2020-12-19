#ifndef RG_SCENE_HPP
#define RG_SCENE_HPP

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

void init(unsigned windowWidth, unsigned windowHeight, const char* title,
          View* camera, Ball* ball,
          std::unordered_map<std::string, std::string>& models,
          std::unordered_map<std::string, ShaderData*>& shaderData);
void loop();
void cleanup();

class Initializer {
private:
    Initializer(unsigned windowWidth, unsigned windowHeight);

    static inline View* makeCamera(unsigned windowWidth, unsigned windowHeight,
                                   glm::vec3 position, glm::vec3 direction) {
        return new View{position,
                        direction,
                        glm::vec3{0.0f, 1.0f, 0.0f},
                        60.0f,
                        static_cast<float>(windowWidth) / windowHeight * 60.0f,
                        0.1f,
                        100.0f};
    }

public:
    static Initializer oneCamera(unsigned width, unsigned height,
                                 float distance);
    static Initializer fourCameras(unsigned width, unsigned height,
                                   float distance);
    Initializer& set_window_width(unsigned windowWidth);
    Initializer& set_window_height(unsigned windowHeight);
    Initializer& set_window_title(const char* title);
    Initializer& set_camera(unsigned index, View* camera);
    Initializer& set_camera_position(unsigned index, glm::vec3 position);
    Initializer& set_camera_direction(unsigned index, glm::vec3 direction);
    Initializer& set_camera_up(unsigned index, glm::vec3 up);
    Initializer& set_camera_horizontal_fov(unsigned index,
                                           float horizontal_fov);
    Initializer& set_camera_vertical_fov(unsigned index, float vertical_fov);
    Initializer& set_camera_z_near(unsigned index, float z_near);
    Initializer& set_camera_z_far(unsigned index, float z_far);
    Initializer& set_ball(Ball* ball);
    /**
     * Add a shader to be used with this renderer.
     * @param name name of the shader, without path or {.vs/.fs}.glsl
     * extension. Both shaders (vertex and fragment) must exist on
     * filesystem in res/shaders directory.
     * @return this
     */
    Initializer& addShader(const std::string& name, ShaderData* data);
    /**
     * Add a model to be used with this renderer.
     * @param filepath filepath of the model, without res/objects prefix.
     * Model file must exist on filesystem at "res/objects/${filepath}".
     * @return this
     * @see Scene::Builder::addModelToShader(unsigned, unsigned), to
     * associate the model with a shader.
     */
    Initializer& addModel(const std::string& name, const std::string& filepath);

    void init();

private:
    unsigned window_height_ = 800;
    unsigned window_width_ = 800;
    const char* title_ = "Window";

    std::vector<View*> cameras_;
    Ball* ball_ = new Ball(Court(), 0, 0, 2);
    std::unordered_map<std::string, std::string> models_;
    std::unordered_map<std::string, ShaderData*> shaders_;
};
} // namespace rg
#endif // RG_SCENE_HPP
