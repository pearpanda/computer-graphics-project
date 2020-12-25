#ifndef RG_INIT_HPP
#define RG_INIT_HPP

#include <app/state.hpp>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace app {

void init();

// Graphics
// ---------
void initGraphics();
void configureWindow();
void configureRender();
#ifdef ENABLE_DEBUG
void enableDebugLogging();

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id,
                            GLenum severity, GLsizei length,
                            const char* message, const void* userParam);
#endif

// Scene initialization
// --------------------
void initScene();
void initObjects();
void initCameras();
void initShaders();
void initSkybox();
void initModels();
void setScene();
void placeCameras();
void placeObjects();
void placeLights();

// Callbacks
// ---------
void bindCallbacks();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void switches_callback(GLFWwindow* window, int key, int scancode, int action,
                       int mods);

namespace util {

std::string resource(const std::string& path);
std::string readFile(const std::string& path);

} // namespace util

} // namespace app

#endif // RG_INIT_HPP
