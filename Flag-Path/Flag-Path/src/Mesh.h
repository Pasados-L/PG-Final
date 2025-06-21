#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Shader.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    glm::ivec4 BoneIDs;   // layout(location = 3)
    glm::vec4 Weights;    // layout(location = 4)

    glm::vec3 PosMorph1;  // layout(location = 5)
    glm::vec3 PosMorph2;  // layout(location = 6)
};


struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;
    glm::mat4 transform;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, glm::mat4 transform);
    void Draw(Shader& shader, const glm::mat4& parentModelMatrix);

private:
    unsigned int VBO, EBO;
    void setupMesh();
};
