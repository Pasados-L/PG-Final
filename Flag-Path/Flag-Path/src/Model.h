#pragma once

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const unsigned int MAX_BONES = 100;

struct BoneInfo {
    glm::mat4 offsetMatrix;
    glm::mat4 finalTransformation;

    BoneInfo() : offsetMatrix(1.0f), finalTransformation(1.0f) {}
};

class Model {
public:
    Assimp::Importer importer;
    const aiScene* scene;
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<glm::mat4> finalBoneMatrices;

    bool hasBones = true; // Nueva bandera para huesos

    Model(const std::string& path);

    void Draw(Shader& shader, const glm::mat4& modelMatrix);

    void UpdateAnimation(float timeInSeconds);

    std::map<std::string, BoneInfo> boneInfoMap;
    int boneCount = 0;

    std::vector<Texture> textures_loaded;
    std::map<std::string, int> boneMapping; // nombre → índice

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4& transform);

    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
    // Métodos de animación
    void ReadNodeHeirarchy(float animationTime, const aiNode* node, const glm::mat4& parentTransform);
    const aiNodeAnim* FindNodeAnim(const aiAnimation* animation, const std::string& nodeName);
    glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4& from);
    glm::vec3 InterpolatePosition(float animationTime, const aiNodeAnim* nodeAnim);
    glm::quat InterpolateRotation(float animationTime, const aiNodeAnim* nodeAnim);
    glm::vec3 InterpolateScaling(float animationTime, const aiNodeAnim* nodeAnim);
    int FindPosition(float animationTime, const aiNodeAnim* nodeAnim);
    int FindRotation(float animationTime, const aiNodeAnim* nodeAnim);
    int FindScaling(float animationTime, const aiNodeAnim* nodeAnim);
};
