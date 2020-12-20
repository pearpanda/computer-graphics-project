#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <rg/renderer/camera/Camera.hpp>
#include <rg/renderer/camera/Surface.hpp>
#include <rg/renderer/model/Model.hpp>
#include <rg/renderer/shader/Shader.hpp>
#include <rg/util/common_meshes.hpp>
#include <rg/util/read_file.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>

#include <iostream>
#include <memory>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
std::array<rg::Camera*, 4> cameras{nullptr};
unsigned int active_camera = 0;
bool multiple_cameras = false;
bool enable_toggle_multiple_cameras = true;
std::array<rg::Surface*, 4> surfaces{nullptr};
std::shared_ptr<rg::MeshVertexData> surface_quad;

float sensitivity = 0.003f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void render(const rg::Shader& shader, const rg::Model& model,
            const rg::Camera& camera, const rg::Surface& surface) {
    // don't forget to enable shader before setting uniforms
    shader.bind();
    surface.bind();

    glClearColor(0.2f, 0.4f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // view/projection transformations
    glm::mat4 projection = camera.get_projection_matrix();
    glm::mat4 view = camera.get_view_matrix();
    shader.set("projection", projection);
    shader.set("view", view);

    // render the loaded model
    glm::mat4 model_matrix = glm::mat4(1.0f);
    model_matrix = glm::translate(
            model_matrix,
            glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the
                                          // center of the scene
    model_matrix = glm::scale(
            model_matrix,
            glm::vec3(1.0f, 1.0f, 1.0f)); // it's a bit too big for our scene,
                                          // so scale it down
    shader.set("model", model_matrix);
    model.draw(shader);

    surface.unbind();
}

// Positional rendering: in specific corner of the screen
void render(const rg::Shader& shader, const rg::Surface& surface,
            unsigned int index) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    static const rg::Surface::SubViewDirectives no_transform{
            glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 1.0f}};
    static const glm::vec2 surface_dimensions{1.0f, 1.0f};
    static const std::array<glm::vec2, 4> origins{
            glm::vec2{-0.5f, -0.5f},
            glm::vec2{0.5f, -0.5f},
            glm::vec2{0.5f, 0.5f},
            glm::vec2{-0.5f, 0.5f},
    };
    static const std::array<rg::Surface::DrawDirectives, 4> directives{
            rg::Surface::DrawDirectives{rg::Surface::ScreenDirectives{
                                                origins[0], surface_dimensions},
                                        no_transform},
            rg::Surface::DrawDirectives{rg::Surface::ScreenDirectives{
                                                origins[1], surface_dimensions},
                                        no_transform},
            rg::Surface::DrawDirectives{rg::Surface::ScreenDirectives{
                                                origins[2], surface_dimensions},
                                        no_transform},
            rg::Surface::DrawDirectives{rg::Surface::ScreenDirectives{
                                                origins[3], surface_dimensions},
                                        no_transform}};

    surface.draw(shader, directives[index]);
}

void render(const rg::Shader& shader, const rg::Surface& surface) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    surface.draw(shader);
}

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Initialize cameras and their surfaces
    // -------------------------------------
    cameras[0] = new rg::Camera{glm::vec3{-0.5f, -0.5f, 3.0f},
                                glm::vec3{0.0f, 0.0f, -1.0f},
                                60.0f,
                                static_cast<float>(SCR_WIDTH) / SCR_HEIGHT,
                                0.1f,
                                100.0f};
    cameras[1] = new rg::Camera{glm::vec3{0.5f, -0.5f, 3.0f},
                                glm::vec3{0.0f, 0.0f, -1.0f},
                                60.0f,
                                static_cast<float>(SCR_WIDTH) / SCR_HEIGHT,
                                0.1f,
                                100.0f};
    cameras[2] = new rg::Camera{glm::vec3{0.5f, 0.5f, 3.0f},
                                glm::vec3{0.0f, 0.0f, -1.0f},
                                60.0f,
                                static_cast<float>(SCR_WIDTH) / SCR_HEIGHT,
                                0.1f,
                                100.0f};
    cameras[3] = new rg::Camera{glm::vec3{-0.5f, 0.5f, 3.0f},
                                glm::vec3{0.0f, 0.0f, -1.0f},
                                60.0f,
                                static_cast<float>(SCR_WIDTH) / SCR_HEIGHT,
                                0.1f,
                                100.0f};
    surface_quad = rg::util::surfaceQuad();
    for (unsigned int i = 0; i < 4; ++i)
        surfaces[i] = new rg::Surface{SCR_WIDTH, SCR_HEIGHT, surface_quad};

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    auto* shader = new rg::Shader{rg::Shader::compile(
            rg::util::readFile(RESOURCE_DIRECTORY "/shaders/example.vs.glsl"),
            rg::util::readFile(RESOURCE_DIRECTORY "/shaders/example.fs.glsl"))};
    auto* surface_shader = new rg::Shader{rg::Shader::compile(
            rg::util::readFile(RESOURCE_DIRECTORY "/shaders/surface.vs.glsl"),
            rg::util::readFile(RESOURCE_DIRECTORY "/shaders/surface.fs.glsl"))};

    // load models
    // -----------
    stbi_set_flip_vertically_on_load(true);
    auto* model =
            new rg::Model(RESOURCE_DIRECTORY "/objects/backpack/backpack.obj");

    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (multiple_cameras) {
            glEnable(GL_DEPTH_TEST);
            for (unsigned int i = 0; i < 4; ++i)
                render(*shader, *model, *cameras[i], *surfaces[i]);

            glDisable(GL_DEPTH_TEST);
            for (unsigned int i = 0; i < 4; ++i)
                render(*surface_shader, *surfaces[i], i);
        } else {
            glEnable(GL_DEPTH_TEST);
            render(*shader, *model, *cameras[active_camera],
                   *surfaces[active_camera]);

            glDisable(GL_DEPTH_TEST);
            render(*surface_shader, *surfaces[active_camera]);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse
        // moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete model;
    for (auto& camera : cameras)
        delete camera;
    for (auto& surface : surfaces)
        delete surface;
    delete shader;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        if (enable_toggle_multiple_cameras) {
            multiple_cameras = !multiple_cameras;
            enable_toggle_multiple_cameras = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_RELEASE) {
        enable_toggle_multiple_cameras = true;
    }

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        active_camera = 0;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        active_camera = 1;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        active_camera = 2;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        active_camera = 3;

    auto camera = cameras[active_camera];
    auto front = camera->get_direction();
    auto right = camera->get_right();

    float speed = 2.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->move(speed * front);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->move(-speed * front);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->move(-speed * right);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->move(speed * right);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    auto x_position = static_cast<float>(xpos);
    auto y_position = static_cast<float>(ypos);
    if (firstMouse) {
        lastX = x_position;
        lastY = y_position;
        firstMouse = false;
    }

    float x_offset = x_position - lastX;
    float y_offset = y_position - lastY;

    lastX = x_position;
    lastY = y_position;

    float delta_yaw = sensitivity * (-x_offset);
    float delta_pitch = sensitivity * (-y_offset);

    auto camera = cameras[active_camera];
    camera->rotate(delta_yaw, delta_pitch);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
//    camera.ProcessMouseScroll(yoffset);
}
