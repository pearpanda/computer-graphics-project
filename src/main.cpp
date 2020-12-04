#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>
#include <string>

#include <rg/renderer/buffer/VertexBuffer.hpp>
#include <rg/renderer/buffer/IndexBuffer.hpp>
#include <rg/renderer/buffer/VertexLayout.hpp>
#include <rg/renderer/buffer/VertexArray.hpp>

const std::string vs = "#version 460 core"
R"S(
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aCol;

out vec3 col;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
    col = aCol;
}
)S";

const std::string fs = "#version 460 core"
                       R"S(
in vec3 col;
out vec4 fragColor;

void main() {
    fragColor = vec4(col.x, col.y, col.z, 1.0f);
}
)S";

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
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    glUseProgram(shader);

    float vertices[] = {
            // x, y, r, g, b
            -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 1.0f, 1.0f, 0.0f
    };

    unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
    };

    using rg::util::floatVector;
    auto* VA = new rg::VertexArray();
    auto* layout = new rg::VertexLayout{floatVector(2), floatVector(3)};
    auto* VB = new rg::VertexBuffer{vertices, sizeof (vertices)};
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
