#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

int main() {
    // Inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Error al inicializar GLFW\n";
        return -1;
    }

    // Crear ventana GLFW
    GLFWwindow* window = glfwCreateWindow(640, 480, "Test Librerías", nullptr, nullptr);
    if (!window) {
        std::cerr << "No se pudo crear la ventana GLFW\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Inicializar glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "No se pudo inicializar glad\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Usar una función de OpenGL (GL)
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Probar glm
    glm::vec3 a(1.0f, 2.0f, 3.0f);
    glm::vec3 b(4.0f, 5.0f, 6.0f);
    glm::vec3 c = a + b;
    std::cout << "glm::vec3 suma: (" << c.x << ", " << c.y << ", " << c.z << ")\n";

    // Probar Assimp
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("test.obj", aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error al cargar modelo con Assimp: " << importer.GetErrorString() << std::endl;
    }
    else {
        std::cout << "Modelo cargado con éxito usando Assimp." << std::endl;
        std::cout << "Número de mallas: " << scene->mNumMeshes << std::endl;
    }

    // Mostrar ventana por un momento
    glfwSwapBuffers(window);
    glfwPollEvents();
    glfwWaitEventsTimeout(1.0); // Espera 1 segundo

    // Limpiar y salir
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
