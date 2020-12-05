#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>

#include <rg/renderer/buffer/IndexBuffer.hpp>
#include <rg/renderer/buffer/VertexArray.hpp>
#include <rg/renderer/buffer/VertexBuffer.hpp>
#include <rg/renderer/buffer/VertexLayout.hpp>
#include <rg/util/layout_elements.hpp>
#include <rg/renderer/shader/Shader.hpp>

#include <rg/util/read_file.hpp>

int main() {
    if (glfwInit() != GLFW_TRUE)
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    GLFWwindow* window = glfwCreateWindow(600, 600, "Test", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    auto keyCallback = [](GLFWwindow* window, int key, int scancode, int action,
                          int mods) {
      if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
          glfwSetWindowShouldClose(window, GLFW_TRUE);
    };
    glfwSetKeyCallback(window, keyCallback);

    float vertices[] = {
            0.5f * glm::cos(glm::pi<float>() / 2.0f),
            0.5f * glm::sin(glm::pi<float>() / 2.0f),
            0.5f * glm::cos(-5.0f * glm::pi<float>() / 6.0f),
            0.5f * glm::sin(-5.0f * glm::pi<float>() / 6.0f),
            0.5f * glm::cos(-glm::pi<float>() / 6.0f),
            0.5f * glm::sin(-glm::pi<float>() / 6.0f)
    };

    unsigned int indices[] = {0, 1, 2};

    using rg::util::element;
    auto* VA = new rg::VertexArray();
    auto* layout =
            new rg::VertexLayout{element<glm::vec2>()};
    auto* VB = new rg::VertexBuffer{vertices, sizeof (vertices)};
    auto* IB = new rg::IndexBuffer{indices, sizeof (indices) / sizeof (indices[0])};
    VA->recordLayout(*VB, *layout);
    VA->unbind();
    VB->unbind();
    IB->unbind();

    std::string resource_dir = RESOURCE_DIRECTORY;
    std::string vs =
            rg::util::readFile(resource_dir + "/shaders/example.vs.glsl");
    std::string fs =
            rg::util::readFile(resource_dir + "/shaders/example.fs.glsl");

    auto* shader = new rg::Shader{rg::Shader::compile(vs, fs)};
    shader->bind();

    glm::mat2x2 rot{1.0f};
    shader->set("rot", rot);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        double time = glfwGetTime();
        rot = {glm::cos(time), glm::sin(time), -glm::sin(time), glm::cos(time)};
        shader->set("rot", rot);

        shader->bind();
        VA->bind();
        glDrawElements(GL_TRIANGLES, IB->count(), GL_UNSIGNED_INT, nullptr);
        VA->unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete layout;
    delete VA;
    delete IB;
    delete VB;
    // delete shader;

    glfwTerminate();
    return 0;
}
