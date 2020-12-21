#ifndef RG_RENDERER_RENDER_HPP
#define RG_RENDERER_RENDER_HPP

#include <rg/renderer/camera/Surface.hpp>
#include <rg/renderer/camera/View.hpp>
#include <rg/renderer/model/Model.hpp>
#include <rg/renderer/model/Skybox.hpp>
#include <rg/renderer/model/Transform.hpp>
#include <rg/renderer/shader/Shader.hpp>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

namespace rg {

void render(const Shader& shader, const Model& model, const View& eye,
            const Surface& surface, const Transform& transform);
void render(const Shader& shader, const Model& model, const View& eye,
            const Surface& surface, const Transform& transform, float shininess,
            const std::vector<DirectionalLight>& directional_lights,
            const std::vector<PointLight>& point_lights,
            const std::vector<SpotLight>& spotlights);

void render(const Shader& skybox_shader, const Skybox& skybox,
            const View& camera, const Surface& surface);

void render(const Shader& surface_shader, const Surface& surface);
void render(const Shader& surface_shader, const Surface& surface,
            unsigned int index);

void clear();
void clear(const Surface& surface);

} // namespace rg

#endif // RG_RENDERER_RENDER_HPP
