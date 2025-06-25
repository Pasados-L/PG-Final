#pragma once
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>

class Model {
public:
    std::vector<Mesh> meshes;
    std::string directory;

    Model(const std::string& path);
    void Draw(Shader& shader, const glm::mat4& modelMatrix);
    std::vector<std::pair<glm::vec3, glm::vec3>> GetAllAABBs() const;
    std::pair<glm::vec3, glm::vec3> GetGlobalAABB();

private:
    std::vector<Texture> textures_loaded;
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& transform);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
