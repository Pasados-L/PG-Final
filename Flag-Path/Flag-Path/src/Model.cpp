#include "Model.h"
#include "Texture.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

Model::Model(const std::string& path) {
    loadModel(path);
}

void Model::Draw(Shader& shader, const glm::mat4& modelMatrix) {
    for (auto& mesh : meshes) {
        mesh.Draw(shader, modelMatrix);
    }
}

void Model::loadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene, glm::mat4(1.0f));
}

void Model::processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform) {
    // Convert Assimp matrix to glm
    glm::mat4 nodeTransform = parentTransform * glm::transpose(glm::make_mat4(&node->mTransformation.a1));
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene, nodeTransform));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, nodeTransform);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& transform) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.Normal = mesh->HasNormals() ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) : glm::vec3(0.0f);
        vertex.TexCoords = mesh->mTextureCoords[0] ?
            glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f);
        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            indices.push_back(mesh->mFaces[i].mIndices[j]);
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    auto normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    return Mesh(vertices, indices, textures, transform);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string path = str.C_Str();
        bool skip = false;
        for (auto& loaded : textures_loaded) {
            if (loaded.path == path) {
                textures.push_back(loaded);
                skip = true;
                break;
            }
        }
        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(path.c_str(), directory);
            texture.type = typeName;
            texture.path = path;
            textures.push_back(texture);
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}
