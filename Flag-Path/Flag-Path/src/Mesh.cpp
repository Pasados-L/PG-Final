#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, glm::mat4 transform)
    : vertices(vertices), indices(indices), textures(textures), transform(transform) {
    setupMesh();
    // Inicializa AABB con extremos opuestos
    aabbMin = glm::vec3(FLT_MAX);
    aabbMax = glm::vec3(-FLT_MAX);

    for (const auto& vertex : vertices) {
        aabbMin = glm::min(aabbMin, vertex.Position);
        aabbMax = glm::max(aabbMax, vertex.Position);
    }

}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader, const glm::mat4& parentModelMatrix) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string name = textures[i].type;
        std::string number;
        if (name == "diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "specular")
            number = std::to_string(specularNr++);
        else if (name == "normal")
            number = std::to_string(normalNr++);
        shader.setInt("texture_" + name + number, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // Combine parent (global) model matrix with this mesh's transform
    glm::mat4 finalModel = parentModelMatrix * transform;
    shader.setMat4("model", finalModel);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
