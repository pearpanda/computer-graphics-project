#include <rg/renderer/model/Model.hpp>

#include <rg/renderer/model/Texture.hpp>
#include <rg/renderer/model/Vertex.hpp>

#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <spdlog/spdlog.h>

#include <array>
#include <memory>
#include <unordered_map>
#include <vector>

namespace rg {

void Model::draw(const Shader& shader) {
    for (const auto& mesh : meshes_)
        mesh.draw(shader);
}

// tying this data to (otherwise heavy) model is bad for reusability
// consider making a ShaderModel class (or something like that) to hold this
void Model::set_scale_vector(glm::vec3 scale) {
    scale_ = scale;
}
void Model::set_translate_vector(glm::vec3 translate) {
    translate_ = translate;
}
glm::vec3 Model::get_scale_vector() {
    return scale_;
}
glm::vec3 Model::get_translate_vector() {
    return translate_;
}

namespace {

class Loader {
public:
    explicit Loader(const std::string& path)
            : meshes_{}, textures_{}, path_{path} {
        directory_ = path.substr(0, path.find_last_of('/'));
    }

    void loadScene();
    std::vector<Mesh> get_meshes() const {
        return meshes_;
    }

private:
    std::vector<Mesh> meshes_;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures_;
    std::string path_;
    std::string directory_;

    /** @brief Recursively processes the scene and converts it to our model
     * format
     *
     * @param node The current node of the scene being processed
     * @param scene Assimp's representation of a scene
     * @param meshes An array to insert the meshes
     * @param memo Used to prevent the same texture being loaded twice
     */
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    static Vertex processVertex(aiMesh* mesh, unsigned int index);

    std::vector<std::shared_ptr<Texture>>
    loadMaterialTextures(aiMaterial* material, TextureType type);
};

} // namespace

Model::Model(const std::string& path) : meshes_{} {
    Loader loader{path};
    loader.loadScene();
    meshes_ = loader.get_meshes();
}

namespace {

void Loader::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        auto* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes_.emplace_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; ++i)
        processNode(node->mChildren[i], scene);
}

Mesh Loader::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    vertices.reserve(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
        vertices.push_back(processVertex(mesh, i));

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        aiFace polygon = mesh->mFaces[i];
        for (unsigned int j = 0; j < polygon.mNumIndices; ++j)
            indices.push_back(polygon.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        auto diffuse_maps =
                loadMaterialTextures(material, TextureType::DIFFUSE);
        auto specular_maps =
                loadMaterialTextures(material, TextureType::SPECULAR);

        textures.insert(textures.end(), diffuse_maps.begin(),
                        diffuse_maps.end());
        textures.insert(textures.end(), specular_maps.begin(),
                        specular_maps.end());
    }

    VertexArray va;
    va.bind();

    VertexBuffer vb{vertices.data(), static_cast<unsigned int>(vertices.size() *
                                                               sizeof(Vertex))};
    vb.bind();

    IndexBuffer ib{indices};
    ib.bind();

    va.recordLayout(vb, util::layout<Vertex>());
    va.unbind();
    ib.unbind();
    vb.unbind();

    return Mesh{std::make_shared<VertexArray>(std::move(va)),
                std::make_shared<VertexBuffer>(std::move(vb)),
                std::make_shared<IndexBuffer>(std::move(ib)), textures};
}

Vertex Loader::processVertex(aiMesh* mesh, unsigned int index) {
    Vertex v;

    v.position.x = mesh->mVertices[index].x;
    v.position.y = mesh->mVertices[index].y;
    v.position.z = mesh->mVertices[index].z;

    v.normal.x = mesh->mNormals[index].x;
    v.normal.y = mesh->mNormals[index].y;
    v.normal.z = mesh->mNormals[index].z;

    // If the mesh has texture coordinates
    if (mesh->mTextureCoords[0]) {
        v.texture_coordinates.x = mesh->mTextureCoords[0][index].x;
        v.texture_coordinates.y = mesh->mTextureCoords[0][index].y;
    } else {
        v.texture_coordinates = glm::vec2{0.0f, 0.0f};
    }

    return v;
}

void Loader::loadScene() {
    Assimp::Importer importer;
    const aiScene* scene =
            importer.ReadFile(path_, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
        scene->mRootNode == nullptr) {

        spdlog::error("ERROR::ASSIMP: {}", importer.GetErrorString());
        return;
    }

    processNode(scene->mRootNode, scene);
}

std::vector<std::shared_ptr<Texture>>
Loader::loadMaterialTextures(aiMaterial* material, TextureType type) {

    static std::array<aiTextureType, 2> types = {aiTextureType_DIFFUSE,
                                                 aiTextureType_SPECULAR};
    std::vector<std::shared_ptr<Texture>> material_textures;
    aiTextureType ai_type = types[static_cast<unsigned int>(type)];
    for (unsigned int i = 0; i < material->GetTextureCount(ai_type); i++) {
        aiString str;
        material->GetTexture(ai_type, i, &str);
        std::string path = str.C_Str();

        auto it = textures_.find(path);
        if (it != textures_.end()) {
            material_textures.push_back(it->second);
        } else {
            Texture texture{directory_ + "/" + path, type};

            auto ptr = std::make_shared<Texture>(texture);
            textures_.emplace(path, ptr);
            material_textures.push_back(ptr);
        }
    }

    return material_textures;
}

} // namespace

} // namespace rg
