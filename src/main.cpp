#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <rg/renderer/camera/Camera.hpp>
#include <rg/renderer/camera/Surface.hpp>
#include <rg/renderer/model/Model.hpp>
#include <rg/renderer/model/Skybox.hpp>
#include <rg/renderer/render.hpp>
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
const unsigned int SCR_WIDTH = 1366;
const unsigned int SCR_HEIGHT = 768;

// skybox
rg::Skybox* box = nullptr;

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

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id,
                            GLenum severity, GLsizei length,
                            const char* message, const void* userParam);

int main() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL",
                                          nullptr, nullptr);
    if (window == nullptr) {
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

    const std::vector<std::string> faces{"xpos.png", "xneg.png", "ypos.png",
                                         "yneg.png", "zpos.png", "zneg.png"};

    box = new rg::Skybox(RESOURCE_DIRECTORY "/skyboxes/night-real/", faces);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // Debug messages
    // --------------
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                          GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

    // build and compile shaders
    // -------------------------
    auto* shader = new rg::Shader{rg::Shader::compile(
            rg::util::readFile(RESOURCE_DIRECTORY "/shaders/example.vs.glsl"),
            rg::util::readFile(RESOURCE_DIRECTORY "/shaders/example.fs.glsl"))};
    auto* surface_shader = new rg::Shader{rg::Shader::compile(
            rg::util::readFile(RESOURCE_DIRECTORY "/shaders/surface.vs.glsl"),
            rg::util::readFile(RESOURCE_DIRECTORY "/shaders/surface.fs.glsl"))};
    auto* skybox_shader = new rg::Shader{rg::Shader::compile(
            rg::util::readFile(RESOURCE_DIRECTORY "/shaders/skybox.vs.glsl"),
            rg::util::readFile(RESOURCE_DIRECTORY "/shaders/skybox.fs.glsl"))};

    // load models
    // -----------
    stbi_set_flip_vertically_on_load(true);
    auto* model =
            new rg::Model(RESOURCE_DIRECTORY "/objects/backpack/backpack.obj");

    // draw in wireframe
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
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
            for (unsigned int i = 0; i < 4; ++i) {
                render(*shader, *model, cameras[i]->get_view(), *surfaces[i]);
                render(*skybox_shader, *box, cameras[i]->get_view(),
                       *surfaces[i]);
            }

            glDisable(GL_DEPTH_TEST);
            for (unsigned int i = 0; i < 4; ++i)
                render(*surface_shader, *surfaces[i], i);
        } else {
            glEnable(GL_DEPTH_TEST);
            render(*shader, *model, cameras[active_camera]->get_view(),
                   *surfaces[active_camera]);
            render(*skybox_shader, *box, cameras[active_camera]->get_view(),
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
    delete box;
    for (auto& camera : cameras)
        delete camera;
    for (auto& surface : surfaces)
        delete surface;
    delete skybox_shader;
    delete surface_shader;
    delete shader;

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window) {
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

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
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
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    //    camera.ProcessMouseScroll(yoffset);
}

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id,
                            GLenum severity, GLsizei length,
                            const char* message, const void* userParam) {
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            std::cout << "Source: API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cout << "Source: Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cout << "Source: Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cout << "Source: Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cout << "Source: Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            std::cout << "Source: Other";
            break;
    }
    std::cout << std::endl;

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cout << "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cout << "Type: Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cout << "Type: Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cout << "Type: Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cout << "Type: Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cout << "Type: Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cout << "Type: Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cout << "Type: Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cout << "Type: Other";
            break;
    }
    std::cout << std::endl;

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cout << "Severity: high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cout << "Severity: medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cout << "Severity: low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::cout << "Severity: notification";
            break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}
