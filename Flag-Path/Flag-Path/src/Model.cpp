#include "Model.h"
#include "Texture.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <stb_image.h>
#include "Shader.h"
void SetVertexBoneData(Vertex& vertex, int boneID, float weight) {
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
        if (vertex.Weights[i] == 0.0f) {
            vertex.BoneIDs[i] = boneID;
            vertex.Weights[i] = weight;
            return;
        }
    }
}

Model::Model(const std::string& path) {
    loadModel(path);
}
void Model::Draw(Shader& shader, const glm::mat4& modelMatrix) {
    if (hasBones) {
        for (int i = 0; i < finalBoneMatrices.size(); i++) {
            std::string name = "finalBonesMatrices[" + std::to_string(i) + "]";
            shader.setMat4(name, finalBoneMatrices[i]);
        }
    }
    for (auto& mesh : meshes) {
        mesh.Draw(shader, modelMatrix);
    }
}
std::vector<std::pair<glm::vec3, glm::vec3>> Model::GetAllAABBs() const {
    std::vector<std::pair<glm::vec3, glm::vec3>> boxes;
    for (const auto& mesh : meshes) {
        boxes.push_back({ mesh.aabbMin, mesh.aabbMax });
    }
    return boxes;
}
std::pair<glm::vec3, glm::vec3> Model::GetGlobalAABB() {
    glm::vec3 min, max;
    // Esto depende de c�mo almacenes las AABBs
    bool first = true;
    for (const auto& box : GetAllAABBs()) {
        if (first) {
            min = box.first;
            max = box.second;
            first = false;
        }
        else {
            min = glm::min(min, box.first);
            max = glm::max(max, box.second);
        }
    }
    return { min, max };
}


void Model::loadModel(std::string path) {
    scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
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
    std::vector<Vertex> vertices(mesh->mNumVertices);
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Inicializar vértices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex& vertex = vertices[i];
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.Normal = mesh->HasNormals() ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z) : glm::vec3(0.0f);
        vertex.TexCoords = mesh->mTextureCoords[0] ?
            glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y) : glm::vec2(0.0f);

        // Inicializa huesos a cero
        for (int j = 0; j < MAX_BONE_INFLUENCE; j++) {
            vertex.BoneIDs[j] = -1;
            vertex.Weights[j] = 0.0f;
        }
    }

    // Procesar huesos (animación esqueletal)
    if (mesh->HasBones()) {
        for (unsigned int i = 0; i < mesh->mNumBones; ++i) {
            std::string boneName(mesh->mBones[i]->mName.data);

            int boneID = 0;
            if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
                boneID = boneCount;
                BoneInfo newBoneInfo;
                newBoneInfo.offsetMatrix = aiMatrix4x4ToGlm(mesh->mBones[i]->mOffsetMatrix);
                boneInfoMap[boneName] = newBoneInfo;
                boneCount++;
            }
            else {
                boneID = std::distance(boneInfoMap.begin(), boneInfoMap.find(boneName));
            }

            auto weights = mesh->mBones[i]->mWeights;
            for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; ++j) {
                int vertexID = weights[j].mVertexId;
                float weight = weights[j].mWeight;
                SetVertexBoneData(vertices[vertexID], boneID, weight);
            }
        }
    }

    // Procesar caras
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            indices.push_back(mesh->mFaces[i].mIndices[j]);
    }

    // Procesar materiales
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    auto normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    return Mesh(vertices, indices, textures, transform);
}
std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) {
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string path = str.C_Str();

        // ❌ Ignorar texturas embebidas
        if (!path.empty() && path[0] == '*') {
            std::cout << "Skipping embedded texture: " << path << std::endl;
            continue;
        }

        // ✅ Cargar solo texturas externas normales
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


glm::mat4 Model::aiMatrix4x4ToGlm(const aiMatrix4x4& from) {
    glm::mat4 to;
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    return to;
}
const aiNodeAnim* Model::FindNodeAnim(const aiAnimation* anim, const std::string& name) {
    for (unsigned i = 0; i < anim->mNumChannels; i++) {
        if (name == anim->mChannels[i]->mNodeName.data)
            return anim->mChannels[i];
    }
    return nullptr;
}

// Helpers: FindPosition, FindRotation, FindScaling omitidos por brevedad – básicamente recorren keyframes del canal y retornan índice apropiado.

glm::vec3 Model::InterpolatePosition(float t, const aiNodeAnim* chan) {
    if (chan->mNumPositionKeys == 1)
        return glm::vec3(chan->mPositionKeys[0].mValue.x, chan->mPositionKeys[0].mValue.y, chan->mPositionKeys[0].mValue.z);
    int idx = FindPosition(t, chan);
    int next = idx + 1;
    float dt = (float)(chan->mPositionKeys[next].mTime - chan->mPositionKeys[idx].mTime);
    float factor = (t - chan->mPositionKeys[idx].mTime) / dt;
    const aiVector3D& start = chan->mPositionKeys[idx].mValue;
    const aiVector3D& end = chan->mPositionKeys[next].mValue;
    return glm::mix(glm::vec3(start.x, start.y, start.z), glm::vec3(end.x, end.y, end.z), factor);
}

int Model::FindPosition(float animationTime, const aiNodeAnim* nodeAnim) {
    for (int i = 0; i < nodeAnim->mNumPositionKeys - 1; i++) {
        if (animationTime < nodeAnim->mPositionKeys[i + 1].mTime)
            return i;
    }
    return 0;
}

int Model::FindRotation(float animationTime, const aiNodeAnim* nodeAnim) {
    for (int i = 0; i < nodeAnim->mNumRotationKeys - 1; i++) {
        if (animationTime < nodeAnim->mRotationKeys[i + 1].mTime)
            return i;
    }
    return 0;
}

int Model::FindScaling(float animationTime, const aiNodeAnim* nodeAnim) {
    for (int i = 0; i < nodeAnim->mNumScalingKeys - 1; i++) {
        if (animationTime < nodeAnim->mScalingKeys[i + 1].mTime)
            return i;
    }
    return 0;
}

glm::quat Model::InterpolateRotation(float animationTime, const aiNodeAnim* nodeAnim) {
    if (nodeAnim->mNumRotationKeys == 1) {
        auto& q = nodeAnim->mRotationKeys[0].mValue;
        return glm::quat(q.w, q.x, q.y, q.z);
    }
    int index = FindRotation(animationTime, nodeAnim);
    int nextIndex = index + 1;
    float deltaTime = (float)(nodeAnim->mRotationKeys[nextIndex].mTime - nodeAnim->mRotationKeys[index].mTime);
    float factor = (animationTime - (float)nodeAnim->mRotationKeys[index].mTime) / deltaTime;
    auto& startQuat = nodeAnim->mRotationKeys[index].mValue;
    auto& endQuat = nodeAnim->mRotationKeys[nextIndex].mValue;

    glm::quat start = glm::quat(startQuat.w, startQuat.x, startQuat.y, startQuat.z);
    glm::quat end = glm::quat(endQuat.w, endQuat.x, endQuat.y, endQuat.z);

    return glm::slerp(start, end, factor);
}

glm::vec3 Model::InterpolateScaling(float animationTime, const aiNodeAnim* nodeAnim) {
    if (nodeAnim->mNumScalingKeys == 1) {
        auto& v = nodeAnim->mScalingKeys[0].mValue;
        return glm::vec3(v.x, v.y, v.z);
    }
    int index = FindScaling(animationTime, nodeAnim);
    int nextIndex = index + 1;
    float deltaTime = (float)(nodeAnim->mScalingKeys[nextIndex].mTime - nodeAnim->mScalingKeys[index].mTime);
    float factor = (animationTime - (float)nodeAnim->mScalingKeys[index].mTime) / deltaTime;
    auto& start = nodeAnim->mScalingKeys[index].mValue;
    auto& end = nodeAnim->mScalingKeys[nextIndex].mValue;
    return glm::mix(glm::vec3(start.x, start.y, start.z), glm::vec3(end.x, end.y, end.z), factor);
}
void Model::UpdateAnimation(float timeInSeconds) {
    if (!scene || !scene->HasAnimations()) return;

    const aiAnimation* animation = scene->mAnimations[0]; // Suponemos una animación
    float ticksPerSecond = animation->mTicksPerSecond != 0 ? (float)animation->mTicksPerSecond : 25.0f;
    float timeInTicks = timeInSeconds * ticksPerSecond;
    float animationTime = fmod(timeInTicks, (float)animation->mDuration);

    if (hasBones) {
        finalBoneMatrices.resize(MAX_BONES, glm::mat4(1.0f));
        ReadNodeHeirarchy(animationTime, scene->mRootNode, glm::mat4(1.0f));

        // Copiar las matrices finales de los huesos a finalBoneMatrices
        int i = 0;
        for (auto& pair : boneInfoMap) {
            if (i >= MAX_BONES) break;
            finalBoneMatrices[i] = pair.second.finalTransformation;
            i++;
        }
    }
    else {
        // Sin huesos: solo animar nodos y actualizar matrices de transformacion
        // Se podría guardar la matriz transform del nodo raíz para usarla al dibujar
        ReadNodeHeirarchy(animationTime, scene->mRootNode, glm::mat4(1.0f));
        // En este caso, puedes almacenar transformaciones para usarlas en Draw, o dejar que cada mesh tenga su transform animado
    }
}
void Model::ReadNodeHeirarchy(float animationTime, const aiNode* node, const glm::mat4& parentTransform) {
    std::string nodeName(node->mName.data);

    glm::mat4 nodeTransformation = aiMatrix4x4ToGlm(node->mTransformation);

    const aiAnimation* animation = scene->mAnimations[0];
    const aiNodeAnim* nodeAnim = FindNodeAnim(animation, nodeName);

    if (nodeAnim) {
        glm::vec3 scaling = InterpolateScaling(animationTime, nodeAnim);
        glm::mat4 scalingM = glm::scale(glm::mat4(1.0f), scaling);

        glm::quat rotationQ = InterpolateRotation(animationTime, nodeAnim);
        glm::mat4 rotationM = glm::mat4_cast(rotationQ);

        glm::vec3 translation = InterpolatePosition(animationTime, nodeAnim);
        glm::mat4 translationM = glm::translate(glm::mat4(1.0f), translation);

        nodeTransformation = translationM * rotationM * scalingM;
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransformation;

    if (hasBones) {
        if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
            BoneInfo& bone = boneInfoMap[nodeName];
            bone.finalTransformation = globalTransformation * bone.offsetMatrix;
        }
    }
    else {
        // Aquí puedes guardar transformaciones para animaciones sin huesos
        // Por ejemplo, actualizar la transformación de las mallas o guardarla en un mapa para usar luego en Draw
        // Esto depende de cómo gestiones los transforms en meshes sin huesos
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ReadNodeHeirarchy(animationTime, node->mChildren[i], globalTransformation);
    }
}


