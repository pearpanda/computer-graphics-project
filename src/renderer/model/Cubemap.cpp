#include <rg/renderer/model/Cubemap.hpp>

#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <stb/stb_image.h>

#include <stdexcept>

namespace rg {

Cubemap::Cubemap(const std::string& path, const std::vector<std::string>& faces)
        : texture_id_{0} {
    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);

    for (unsigned int i = 0; i < faces.size(); ++i) {
        int width, height, channels;
        unsigned char* data = nullptr;

        std::string full_path = path + faces[i];
        data = stbi_load(full_path.c_str(), &width, &height, &channels, 3);
        if (!data) {
            spdlog::error(
                    "STBI::ERROR::CUBEMAP: Failed to load 2D texture at: {}",
                    full_path);
        } else {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB8, width,
                         height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Cubemap::Cubemap() : texture_id_{0} {
}

Cubemap::~Cubemap() {
    glDeleteTextures(1, &texture_id_);
}

Cubemap::Cubemap(Cubemap&& other) noexcept {
    texture_id_ = other.texture_id_;
    other.texture_id_ = 0;
}

Cubemap& Cubemap::operator=(Cubemap&& other) noexcept {
    texture_id_ = other.texture_id_;
    other.texture_id_ = 0;
    return *this;
}

unsigned int Cubemap::get_cubemap_texture() const {
    return texture_id_;
}

void Cubemap::bind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id_);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void Cubemap::unbind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

} // namespace rg
