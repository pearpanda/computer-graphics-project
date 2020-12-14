#include <glad/glad.h>
#include <glm/ext/quaternion_float.hpp>
#include <rg/renderer/Renderer.hpp>
#include <rg/util/read_file.hpp>
#include <spdlog/spdlog.h>
#include <stb/stb_image.h>
#include <utility>

namespace rg {
struct Renderer::Callbacks {
    // process all input: query GLFW whether relevant keys are pressed/released
    // this frame and react accordingly
    // ---------------------------------------------------------------------------------------------------------

    // glfw: whenever the window_ size changed (by OS or user resize) this
    // callback function executes
    // ---------------------------------------------------------------------------------------------
    static void framebuffer_size_callback(GLFWwindow* window, int width,
                                          int height) {
        // make sure the viewport matches the new window_ dimensions; note that
        // width and height will be significantly larger than specified on
        // retina displays.
        glViewport(0, 0, width, height);
    }

    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
        auto inst = Renderer::instance_;
        if (inst->first_mouse_) {
            inst->last_x_ = xpos;
            inst->last_y_ = ypos;
            inst->first_mouse_ = false;
        }

        float xoffset = xpos - inst->last_x_;
        float yoffset =
                inst->last_y_ -
                ypos; // reversed since y-coordinates go from bottom to top

        inst->last_x_ = xpos;
        inst->last_y_ = ypos;

        float delta_yaw = inst->sensitivity_ * (-xoffset);
        float delta_pitch = inst->sensitivity_ * (-yoffset);

        inst->yaw_ += delta_yaw;
        inst->pitch_ += delta_pitch;

        inst->pitch_ =
                glm::clamp(inst->pitch_, -glm::pi<float>() / 2.0f + 0.01f,
                           glm::pi<float>() / 2.0f - 0.01f);
        inst->yaw_ = glm::mod(inst->yaw_, 2 * glm::pi<float>());

        auto quat = glm::quat(glm::vec3{inst->pitch_, inst->yaw_, 0.0f});
        inst->camera_->direction = quat * glm::vec3{0.0f, 0.0f, 1.0f};
        inst->camera_->up = quat * glm::vec3{0.0f, 1.0f, 0.0f};
    }

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    static void scroll_callback(GLFWwindow* window, double xoffset,
                                double yoffset) {
        //    camera_.ProcessMouseScroll(yoffset);
    }
};

Renderer::Renderer(
        unsigned int windowWidth, unsigned int windowHeight, const char* title,
        rg::View* camera, const std::vector<std::string>& shaderNames,
        const std::vector<std::string>& modelFiles,
        std::unordered_map<unsigned, std::vector<unsigned>> modelMappings) {
    this->model_mappings_ = std::move(modelMappings);
    this->camera_ = camera;
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
    glfwSetFramebufferSizeCallback(window_,
                                   Callbacks::framebuffer_size_callback);
    glfwSetCursorPosCallback(window_, Callbacks::mouse_callback);
    glfwSetScrollCallback(window_, Callbacks::scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    // build and compile shaders_
    // -------------------------
    for (const std::string& shaderName : shaderNames) {
        shaders_.push_back(new Shader{rg::Shader::compile(
                rg::util::readFile(RESOURCE_DIRECTORY "/shaders/" + shaderName +
                                   ".vs.glsl"),
                rg::util::readFile(RESOURCE_DIRECTORY "/shaders/" + shaderName +
                                   ".fs.glsl"))});
    }

    // load models
    // -----------
    for (const std::string& modelFile : modelFiles) {
        models_.push_back(
                new Model(RESOURCE_DIRECTORY "/objects/" + modelFile));
    }
}

void Renderer::loop() {
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
        for (int i = 0; i < shaders_.size(); i++) {
            Shader* shader = shaders_[i];
            shader->bind();

            // view/projection transformations
            glm::mat4 projection = camera_->get_projection_matrix();
            glm::mat4 view = camera_->get_view_matrix();
            shader->set("projection", projection);
            shader->set("view", view);

            // render the loaded model
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,
                                   glm::vec3(5.0f, 15.0f,
                                             0.0f)); // translate it so it's at
                                                     // the center of the scene
            model = glm::scale(model,
                               glm::vec3(1.0f, 1.0f,
                                         1.0f)); // it's a bit too big for our
                                                 // scene, so scale it down
            int modelCount = 0;
            for (int modelIndex : model_mappings_[i]) {
                shader->set("model" + std::to_string(modelCount), model);
                models_[modelIndex]->draw(*shader);
                modelCount++;
            }
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse
        // moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void Renderer::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float speed = 2.0f * delta_time_;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera_->position += speed * camera_->direction;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera_->position -= speed * camera_->direction;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera_->position -=
                speed * glm::cross(camera_->direction, camera_->up);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera_->position +=
                speed * glm::cross(camera_->direction, camera_->up);
}

Renderer::~Renderer() {
    for (auto model : models_)
        delete model;
    for (auto shader : shaders_)
        delete shader;
    delete camera_;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

Renderer::Builder::Builder() {
    if (Renderer::instance_ != nullptr) {
        // constructing multiple renderers is a no-no, so we'll cut it out
        // right away. Realistically this should never be a problem (if it
        // becomes, explore using a proper Singleton or something similar).
        throw std::runtime_error{
                "There cannot be more than one renderer active!"};
    }
}

Renderer::Builder*
Renderer::Builder::set_window_height(unsigned int windowHeight) {
    window_height_ = windowHeight;
    return this;
}
Renderer::Builder*
Renderer::Builder::set_window_width(unsigned int windowWidth) {
    window_width_ = windowWidth;
    return this;
}
Renderer::Builder* Renderer::Builder::set_window_title(const char* title) {
    title_ = title;
    return this;
}

Renderer::Builder* Renderer::Builder::set_camera(View* camera) {
    camera_ = camera;
    return this;
}
Renderer::Builder* Renderer::Builder::set_camera_position(glm::vec3 position) {
    camera_->position = position;
    return this;
}
Renderer::Builder*
Renderer::Builder::set_camera_direction(glm::vec3 direction) {
    camera_->direction = direction;
    return this;
}
Renderer::Builder* Renderer::Builder::set_camera_up(glm::vec3 up) {
    camera_->up = up;
    return this;
}
Renderer::Builder*
Renderer::Builder::set_camera_horizontal_fov(float horizontal_fov) {
    camera_->horizontal_fov = horizontal_fov;
    return this;
}
Renderer::Builder*
Renderer::Builder::set_camera_vertical_fov(float vertical_fov) {
    camera_->vertical_fov = vertical_fov;
    return this;
}
Renderer::Builder* Renderer::Builder::set_camera_z_near(float z_near) {
    camera_->z_near = z_near;
    return this;
}
Renderer::Builder* Renderer::Builder::set_camera_z_far(float z_far) {
    camera_->z_far = z_far;
    return this;
}

Renderer::Builder* Renderer::Builder::addShader(const std::string& name) {
    shader_names_.push_back(name);
    return this;
}
Renderer::Builder* Renderer::Builder::addModel(const std::string& filepath) {
    model_files_.push_back(filepath);
    return this;
}
Renderer::Builder* Renderer::Builder::addModelToShader(unsigned shaderIndex,
                                                       unsigned modelIndex) {
    model_mappings_[shaderIndex].push_back(modelIndex);
    return this;
}

Renderer* Renderer::Builder::build() {
    if (Renderer::instance_ != nullptr) {
        // we've checked this in constructor, but maybe someone has constructed
        // multiple builders and hasn't finished any.
        // this is obviously not thread-safe.
        throw std::runtime_error{
                "There cannot be more than one renderer active!"};
    }

    auto renderer = new Renderer(window_width_, window_height_, title_, camera_,
                                 shader_names_, model_files_, model_mappings_);
    Renderer::instance_ = renderer;
    return renderer;
}
} // namespace rg
