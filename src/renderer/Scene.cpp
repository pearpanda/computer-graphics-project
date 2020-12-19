#include <glad/glad.h>
#include <glm/ext/quaternion_float.hpp>
#include <rg/renderer/Scene.hpp>
#include <rg/util/read_file.hpp>
#include <spdlog/spdlog.h>
#include <stb/stb_image.h>
#include <utility>

namespace rg {
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

bool initialized = false;

GLFWwindow* window_;
std::vector<View*> cameras_;
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

void init(unsigned windowWidth, unsigned windowHeight, const char* title,
          std::vector<View*> cameras, Ball* ball,
          std::unordered_map<std::string, std::string>& models,
          std::unordered_map<std::string, ShaderData*>& shaderData) {
    if (initialized) {
        throw std::runtime_error{"Scene was already initialized!"};
    }
    cameras_ = std::move(cameras);
    ball_ = ball;
    last_x_ = windowWidth / 2.0f;
    last_y_ = windowHeight / 2.0f;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window_ creation
    // --------------------
    window_ = glfwCreateWindow(windowWidth, windowHeight, title, nullptr,
                               nullptr);
    if (window_ == nullptr) {
        spdlog::critical("Failed to create GLFW window");
        glfwTerminate();
        throw std::runtime_error{"Error initializing window"};
    }
    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
    glfwSetCursorPosCallback(window_, mouse_callback);
    glfwSetScrollCallback(window_, scroll_callback);

#ifndef DEBUG
    // tell GLFW to capture our mouse
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        spdlog::critical("Failed to initialize GLAD");
        throw std::runtime_error{"Failed to initialize GLAD"};
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading
    // model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    for (const auto& shaderDatum : shaderData) {
        const std::string shaderName = shaderDatum.first;
        auto* shader = new Shader{rg::Shader::compile(
                rg::util::readFile(RESOURCE_DIRECTORY "/shaders/" + shaderName +
                                   ".vs.glsl"),
                rg::util::readFile(RESOURCE_DIRECTORY "/shaders/" + shaderName +
                                   ".fs.glsl"))};
        shaders_[shader] = shaderDatum.second;
    }

    // load models
    // -----------
    for (auto& modelDatum : models) {
        auto* model =
                new Model(modelDatum.first,
                          RESOURCE_DIRECTORY "/objects/" + modelDatum.second);
        models_[modelDatum.first] = model;
    }

    initialized = true;
}

void loop() {
    auto camera = cameras_[0]; // todo support for multiple cameras
    std::unordered_map<std::string, unsigned> lightCounts;
    while (!glfwWindowShouldClose(window_)) {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        delta_time_ = currentFrame - last_frame_;
        last_frame_ = currentFrame;

        // input
        // -----
        processInput(window_);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        for (const auto& shaderData : shaders_) {
            Shader* shader = shaderData.first;
            shader->bind();

            // view/projection transformations
            glm::mat4 projection = camera->get_projection_matrix();
            glm::mat4 view = camera->get_view_matrix();
            shader->set("projection", projection);
            shader->set("view", view);
            shader->set("viewPosition", camera->position);

            // set lights
            for (const auto& light : shaderData.second->get_lights()) {
                const std::string& type = light->getShaderFieldName();
                light->apply(shader, type + "[" +
                                             std::to_string(lightCounts[type]) +
                                             "]");
                lightCounts[type]++;
            }
            for (const auto& lightCount : lightCounts) {
                shader->set(lightCount.first + "Size", lightCount.second);
            }

            // render the loaded model
            int modelCount = 0;
            auto models = shaderData.second->get_models();
            for (auto& modelData : models) {
                shader->bind(); // if this is not the first iteration,
                                // model->draw has unbound the shader; we need
                                // to activate it again to set uniforms
                auto modelName = modelData.first;
                auto model = models_[modelName];
                glm::mat4 modelTransform = glm::mat4(1.0f);
                modelTransform = glm::translate(modelTransform,
                                                modelData.second->translate_);
                modelTransform =
                        glm::scale(modelTransform, modelData.second->scale_);
                shader->set("model", modelTransform);
                shader->set("material.shininess", modelData.second->shininess_);

                model->draw(*shader);
                modelCount++;
            }
            for (auto& lc : lightCounts)
                lc.second = 0;
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse
        // moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    auto camera = cameras_[0];
    float speed = 2.0f * delta_time_;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->position += speed * camera->direction;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->position -= speed * camera->direction;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->position -= speed * glm::cross(camera->direction, camera->up);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->position += speed * glm::cross(camera->direction, camera->up);
}

void cleanup() {
    for (const auto& model : models_) {
        delete model.second;
    }
    for (const auto& shader : shaders_) {
        delete shader.first;
        delete shader.second;
    }
    for (const auto& camera : cameras_) {
        delete camera;
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    initialized = false;
}

// process all input: query GLFW whether relevant keys are pressed/released
// this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------

// glfw: whenever the window_ size changed (by OS or user resize) this
// callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window_ dimensions; note that
    // width and height will be significantly larger than specified on
    // retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (first_mouse_) {
        last_x_ = xpos;
        last_y_ = ypos;
        first_mouse_ = false;
    }

    float xoffset = xpos - last_x_;
    float yoffset = last_y_ - ypos; // reversed since y-coordinates go from
    // bottom to top

    last_x_ = xpos;
    last_y_ = ypos;

    float delta_yaw = sensitivity_ * (-xoffset);
    float delta_pitch = sensitivity_ * (-yoffset);

    yaw_ += delta_yaw;
    pitch_ += delta_pitch;

    pitch_ = glm::clamp(pitch_, -glm::pi<float>() / 2.0f + 0.01f,
                        glm::pi<float>() / 2.0f - 0.01f);
    yaw_ = glm::mod(yaw_, 2 * glm::pi<float>());

    auto quat = glm::quat(glm::vec3{pitch_, yaw_, 0.0f});
    cameras_[0]->direction = quat * glm::vec3{0.0f, 0.0f, 1.0f};
    cameras_[0]->up = quat * glm::vec3{0.0f, 1.0f, 0.0f};
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    //    camera_.ProcessMouseScroll(yoffset);
}

Initializer Initializer::oneCamera(unsigned width, unsigned height,
                                   float distance) {
    View* camera = makeCamera(width, height, glm::vec3{-1.f, 2.f, -distance},
                              glm::vec3{0.f, 0.f, 1.f});
    Initializer initializer(width, height);
    initializer.cameras_.push_back(camera);
    return initializer;
}
Initializer Initializer::fourCameras(unsigned width, unsigned height,
                                     float distance) {
    Initializer initializer(width, height);
    float d = distance;
    View* camera1 = makeCamera(width, height, glm::vec3{d, d, d},
                               glm::vec3{-d, -d, -d});
    initializer.cameras_.push_back(camera1);
    View* camera2 = makeCamera(width, height, glm::vec3{-d, -d, d},
                               glm::vec3{d, d, -d});
    initializer.cameras_.push_back(camera2);
    View* camera3 = makeCamera(width, height, glm::vec3{d, d, -d},
                               glm::vec3{-d, -d, d});
    initializer.cameras_.push_back(camera3);
    View* camera4 = makeCamera(width, height, glm::vec3{-d, -d, -d},
                               glm::vec3{d, d, d});
    initializer.cameras_.push_back(camera4);
    return initializer;
}

Initializer::Initializer(unsigned width, unsigned height) {
    if (initialized) {
        throw std::runtime_error{"Scene was already initialized! "};
    }
    window_width_ = width;
    window_height_ = height;
}

Initializer& Initializer::set_window_height(unsigned int windowHeight) {
    window_height_ = windowHeight;
    return *this;
}
Initializer& Initializer::set_window_width(unsigned int windowWidth) {
    window_width_ = windowWidth;
    return *this;
}
Initializer& Initializer::set_window_title(const char* title) {
    title_ = title;
    return *this;
}

Initializer& Initializer::set_camera(unsigned index, View* camera) {
    if (cameras_.size() <= index) {
        cameras_.push_back(camera);
    } else {
        delete cameras_[index];
        cameras_[index] = camera;
    }
    return *this;
}
Initializer& Initializer::set_camera_position(unsigned index,
                                              glm::vec3 position) {
    cameras_.at(index)->position = position;
    return *this;
}
Initializer& Initializer::set_camera_direction(unsigned index,
                                               glm::vec3 direction) {
    cameras_.at(index)->direction = direction;
    return *this;
}
Initializer& Initializer::set_camera_up(unsigned index, glm::vec3 up) {
    cameras_.at(index)->up = up;
    return *this;
}
Initializer& Initializer::set_camera_horizontal_fov(unsigned index,
                                                    float horizontal_fov) {
    cameras_.at(index)->horizontal_fov = horizontal_fov;
    return *this;
}
Initializer& Initializer::set_camera_vertical_fov(unsigned index,
                                                  float vertical_fov) {
    cameras_.at(index)->vertical_fov = vertical_fov;
    return *this;
}
Initializer& Initializer::set_camera_z_near(unsigned index, float z_near) {
    cameras_.at(index)->z_near = z_near;
    return *this;
}
Initializer& Initializer::set_camera_z_far(unsigned index, float z_far) {
    cameras_.at(index)->z_far = z_far;
    return *this;
}

Initializer& Initializer::set_ball(Ball* ball) {
    delete ball_;
    ball_ = ball;
    return *this;
}

Initializer& Initializer::addModel(const std::string& name,
                                   const std::string& filepath) {
    models_[name] = filepath;
    return *this;
}

Initializer& Initializer::addShader(const std::string& name, ShaderData* data) {
    shaders_[name] = data;
    // if we actually ever need to associate different models with the same
    // shaders, this'll need to change to be map<string, vector<ShaderData>>
    return *this;
}

void Initializer::init() {
    rg::init(window_width_, window_height_, title_, cameras_, ball_, models_,
             shaders_);
}
} // namespace rg
