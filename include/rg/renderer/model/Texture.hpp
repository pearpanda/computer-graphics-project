#ifndef RG_RENDERER_MODEL_TEXTURE_HPP
#define RG_RENDERER_MODEL_TEXTURE_HPP

#include <string>

namespace rg {

enum class TextureType { DIFFUSE = 0, SPECULAR = 1 };

struct Texture {
    unsigned int texture_id;
    TextureType type;

    Texture(unsigned int id, TextureType type);
    Texture(const std::string& path, TextureType type);
};

} // namespace rg

#endif // RG_RENDERER_MODEL_TEXTURE_HPP
