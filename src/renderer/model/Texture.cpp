#include <rg/renderer/model/Texture.hpp>

#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <stb/stb_image.h>

namespace rg {

Texture::Texture(unsigned int id, TextureType type)
        : texture_id{id}, type{type} {
}

Texture::Texture(const std::string& path, TextureType type)
        : texture_id{0}, type{type} {
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, num_channels, comp;
    auto* data = stbi_load(path.c_str(), &width, &height, &num_channels, 0);
    if (data == nullptr) {
        spdlog::error(
                "ERROR::RG::TEXTURE: Failed to load texture at path \"{}\"",
                path);
    } else {
        switch (num_channels) {
            case 1:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED,
                             GL_UNSIGNED_BYTE, data);
                break;
            case 2:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8, width, height, 0, GL_RG,
                             GL_UNSIGNED_BYTE, data);
                break;
            case 3:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0,
                             GL_RGB, GL_UNSIGNED_BYTE, data);
                break;
            default:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, data);
                break;
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
}

} // namespace rg
