#define STB_EASY_FONT_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "Shader.h"
#include "stb_easy_font.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void drawTextModern(const char* text, float x, float y);

// Variables globales para texto
unsigned int textVAO = 0, textVBO = 0;

void drawTextModern(const char* text, float x, float y) {
    static char buffer[99999];
    int num_quads = stb_easy_font_print(x, y, (char*)text, NULL, buffer, sizeof(buffer));

    if (textVAO == 0) {
        glGenVertexArrays(1, &textVAO);
        glGenBuffers(1, &textVBO);
    }

    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, num_quads * 4 * 16, buffer, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (void*)0);
    glDrawArrays(GL_QUADS, 0, num_quads * 4);

    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Mantener Core Profile

    GLFWwindow* window = glfwCreateWindow(800, 600, "Flag-Path", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    Shader menuShader("menu_vertex.glsl", "menu_fragment.glsl");

    float yOffsets[] = { 0.45f, 0.15f, -0.15f, -0.45f };

    std::vector<float> buttons[4];
    for (int i = 0; i < 4; i++) {
        float y = yOffsets[i];
        std::vector<float> verts = {
            -0.3f, y + 0.1f,
             0.3f, y + 0.1f,
             0.3f, y - 0.1f,
            -0.3f, y - 0.1f
        };
        buttons[i] = verts;
    }

    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    struct Button {
        unsigned int VAO, VBO, EBO;
        float xMin, xMax, yMin, yMax;
    };

    Button buttonsData[4];
    for (int i = 0; i < 4; i++) {
        glGenVertexArrays(1, &buttonsData[i].VAO);
        glGenBuffers(1, &buttonsData[i].VBO);
        glGenBuffers(1, &buttonsData[i].EBO);

        glBindVertexArray(buttonsData[i].VAO);
        glBindBuffer(GL_ARRAY_BUFFER, buttonsData[i].VBO);
        glBufferData(GL_ARRAY_BUFFER, buttons[i].size() * sizeof(float), buttons[i].data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buttonsData[i].EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        buttonsData[i].xMin = -0.3f;
        buttonsData[i].xMax = 0.3f;
        buttonsData[i].yMin = yOffsets[i] - 0.1f;
        buttonsData[i].yMax = yOffsets[i] + 0.1f;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    double xpos, ypos;
    int width = 800, height = 600;
    float xNDC, yNDC;

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.5f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        menuShader.use();

        for (int i = 0; i < 4; i++) {
            glBindVertexArray(buttonsData[i].VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            for (int i = 0; i < 4; i++) {
                if (xNDC >= buttonsData[i].xMin && xNDC <= buttonsData[i].xMax &&
                    yNDC >= buttonsData[i].yMin && yNDC <= buttonsData[i].yMax) {
                    switch (i) {
                    case 0: std::cout << "Jugar\n"; break;
                    case 1: std::cout << "Opciones\n"; break;
                    case 2: std::cout << "Creditos\n"; break;
                    case 3: std::cout << "Salir\n"; break;
                    }
                }
            }
        }

        // Preparar proyección ortográfica para texto
        glUseProgram(0);
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glColor3f(1.0f, 1.0f, 1.0f);

        drawTextModern("JUGAR", 330, 455);
        drawTextModern("OPCIONES", 310, 345);
        drawTextModern("CREDITOS", 310, 245);
        drawTextModern("SALIR", 330, 145);

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glfwGetCursorPos(window, &xpos, &ypos);
        glfwGetWindowSize(window, &width, &height);
        xNDC = (float)(xpos / width) * 2.0f - 1.0f;
        yNDC = 1.0f - (float)(ypos / height) * 2.0f;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
