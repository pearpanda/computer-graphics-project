#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <iostream>
#include <string>

#include <rg/renderer/buffer/IndexBuffer.hpp>
#include <rg/renderer/buffer/VertexArray.hpp>
#include <rg/renderer/buffer/VertexBuffer.hpp>
#include <rg/renderer/buffer/VertexLayout.hpp>
#include <rg/util/layout_elements.hpp>

#include <rg/util/read_file.hpp>

int main() {
    if (glfwInit() != GLFW_TRUE)
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Test", nullptr, nullptr);
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

    std::string resource_dir = RESOURCE_DIRECTORY;
    std::string vs =
            rg::util::readFile(resource_dir + "/shaders/example.vs.glsl");
    std::string fs =
            rg::util::readFile(resource_dir + "/shaders/example.fs.glsl");

    unsigned int VSID = glCreateShader(GL_VERTEX_SHADER);
    const char* text = vs.c_str();
    glShaderSource(VSID, 1, &text, nullptr);
    glCompileShader(VSID);
    unsigned int FSID = glCreateShader(GL_FRAGMENT_SHADER);
    text = fs.c_str();
    glShaderSource(FSID, 1, &text, nullptr);
    glCompileShader(FSID);
    unsigned int shader = glCreateProgram();
    glAttachShader(shader, VSID);
    glAttachShader(shader, FSID);
    glLinkProgram(shader);
    int success;
    char infoLog[512];
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    glUseProgram(shader);

    float vertices[] = {// x, y, r, g, b
                        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.5f, -0.5f,
                        0.0f,  1.0f,  0.0f, 0.5f, 0.5f, 0.0f, 0.0f,
                        1.0f,  -0.5f, 0.5f, 1.0f, 1.0f, 0.0f};

    unsigned int indices[] = {0, 1, 2, 0, 2, 3};

    using rg::util::element;
    auto* VA = new rg::VertexArray();
    auto* layout =
            new rg::VertexLayout{element<glm::vec2>(), element<glm::vec3>()};
    auto* VB = new rg::VertexBuffer{vertices, sizeof(vertices)};
    auto* IB = new rg::IndexBuffer{indices, 6};
    VA->recordLayout(*VB, *layout);
    VA->unbind();
    VB->unbind();
    IB->unbind();

    auto keyCallback = [](GLFWwindow* window, int key, int scancode, int action,
                          int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    };
    glfwSetKeyCallback(window, keyCallback);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

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

    glfwTerminate();
    return 0;
}
