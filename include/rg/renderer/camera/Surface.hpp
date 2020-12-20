#ifndef RG_RENDERER_CAMERA_SURFACE_HPP
#define RG_RENDERER_CAMERA_SURFACE_HPP

#include <rg/renderer/buffer/FrameBuffer.hpp>
#include <rg/renderer/model/Mesh.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <memory>

namespace rg {

class Surface {
public:
    struct ScreenDirectives;
    struct SubViewDirectives;
    class DrawDirectives;

    Surface(unsigned int width, unsigned int height);
    ~Surface();
    void draw(const Shader& shader) const;
    void draw(const Shader& shader, const DrawDirectives& directives) const;
    void bind() const;
    void unbind() const;

    struct ScreenDirectives {
        glm::vec2 origin;
        glm::vec2 dimensions;

        ScreenDirectives(glm::vec2 origin, glm::vec2 dimensions);
        glm::mat4 get_model_matrix() const;
    };

    struct SubViewDirectives {
        glm::vec2 origin;
        glm::vec2 dimensions;

        SubViewDirectives(glm::vec2 origin, glm::vec2 dimensions);
        glm::mat4 get_texture_matrix() const;
    };

    class DrawDirectives {
    public:
        DrawDirectives(ScreenDirectives sd, SubViewDirectives svd);
        glm::mat4 get_model_matrix() const;
        glm::mat4 get_texture_matrix() const;

    private:
        ScreenDirectives sd_;
        SubViewDirectives svd_;
    };

private:
    FrameBuffer fb_;
    Mesh* quad_;
};

} // namespace rg

#endif // RG_RENDERER_CAMERA_SURFACE_HPP
