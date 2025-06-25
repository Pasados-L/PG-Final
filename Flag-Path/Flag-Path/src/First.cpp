#define STB_EASY_FONT_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <cstring>
#include "Shader.h"
#include "stb_easy_font.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void drawTextModern(const char* text, float x, float y);
void drawTextScaledCentered(const char* text, float centerX, float centerY, float scale);

// Variables globales para texto
unsigned int textVAO = 0, textVBO = 0;

void showCredits() {
    GLFWwindow* creditsWindow = glfwCreateWindow(800, 600, "Creditos", NULL, NULL);
    if (!creditsWindow) {
        std::cout << "Error al crear ventana de créditos\n";
        return;
    }

    glfwMakeContextCurrent(creditsWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Fallo en GLAD en créditos\n";
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(creditsWindow)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // Fondo gris oscuro
        glClear(GL_COLOR_BUFFER_BIT);

        // Dibujar rectángulo blanco centrado
        float rect[] = {
            -0.9f,  0.8f,
             0.9f,  0.8f,
             0.9f, -0.8f,
            -0.9f, -0.8f
        };
        unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glColor3f(1.0f, 1.0f, 1.0f); // color blanco para el rectángulo
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glBindVertexArray(0);

        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);

        // Proyección ortográfica en píxeles (800x600)
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glColor3f(0.0f, 0.0f, 0.0f); // texto negro

        //nombres y tamaño ajustable
        const char* names[] = { "             Cajina Jarquin Lisbeth Dayana  2023-0653U","                     Montiel Loaisiga Alvaro Francisco  2023-0708U","                 Sanchez Ramirez Elena Elizabeth  2023-0788U", "                 Gaitan Largaespada Jesus Antonio 2023-0631U" };
        float textScale = 2.5f;      // ← Cambia aquí el tamaño del texto
        float spacing = 50.0f;       // Espacio entre nombres
        float startY = 180.0f;       // Y inicial para el primer nombre

        int count = sizeof(names) / sizeof(names[0]);
        for (int i = 0; i < count; ++i) {
            float centerX = 400.0f;                      // Centro horizontal
            float centerY = startY + i * spacing;        // Posición vertical
            drawTextScaledCentered(names[i], centerX, centerY, textScale);
        }

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glfwSwapBuffers(creditsWindow);
    }

    glfwDestroyWindow(creditsWindow);
}
void showOptions() {
    GLFWwindow* creditsWindow = glfwCreateWindow(800, 600, "Opciones", NULL, NULL);
    if (!creditsWindow) {
        std::cout << "Error al crear ventana de Opciones\n";
        return;
    }

    glfwMakeContextCurrent(creditsWindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Fallo en GLAD en Opciones\n";
        return;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(creditsWindow)) {
        glfwPollEvents();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // Fondo gris oscuro
        glClear(GL_COLOR_BUFFER_BIT);

        // Dibujar rectángulo blanco centrado
        float rect[] = {
            -0.9f,  0.8f,
             0.9f,  0.8f,
             0.9f, -0.8f,
            -0.9f, -0.8f
        };
        unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glColor3f(1.0f, 1.0f, 1.0f); // color blanco para el rectángulo
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glBindVertexArray(0);

        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);

        // Proyección ortográfica en píxeles (800x600)
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glColor3f(0.0f, 0.0f, 0.0f); // texto negro

        //nombres y tamaño ajustable
        const char* names[] = { "     PROXIMAMENTE","   ","              AJUSTES PARA SONIDO E IDIOMA" };
        float textScale = 4.0f;      // ← Cambia aquí el tamaño del texto
        float spacing = 50.0f;       // Espacio entre nombres
        float startY = 180.0f;       // Y inicial para el primer nombre

        int count = sizeof(names) / sizeof(names[0]);
        for (int i = 0; i < count; ++i) {
            float centerX = 400.0f;                      // Centro horizontal
            float centerY = startY + i * spacing;        // Posición vertical
            drawTextScaledCentered(names[i], centerX, centerY, textScale);
        }

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glfwSwapBuffers(creditsWindow);
    }

    glfwDestroyWindow(creditsWindow);
}

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

// ✅ Nueva función para texto escalado y centrado
void drawTextScaledCentered(const char* text, float centerX, float centerY, float scale) {
    int len = strlen(text);
    float baseWidth = len * 8.0f;
    float baseHeight = 13.0f;

    float xText = centerX - (baseWidth * scale) / 2.0f;
    float yText = centerY - (baseHeight * scale) / 2.0f;

    glPushMatrix();
    glTranslatef(xText, yText, 0.0f);
    glScalef(scale, scale, 1.0f);
    drawTextModern(text, 0, 0);
    glPopMatrix();
}

extern int program();
void showCredits();  // Declaración


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

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

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                if (xNDC >= buttonsData[i].xMin && xNDC <= buttonsData[i].xMax &&
                    yNDC >= buttonsData[i].yMin && yNDC <= buttonsData[i].yMax) {
                    switch (i) {
                    case 0:
                        std::cout << "Jugar\n";
                        program();
                        break;
                    case 1:
                        std::cout << "Opciones\n";
                        showOptions();
                        break;
                    case 2:
                        std::cout << "Créditos\n";
                        showCredits();
                        break;
                    case 3:
                        std::cout << "Salir\n";
                        glfwSetWindowShouldClose(window, true);
                        break;
                    }
                }
            }
        }

        // Dibujo de texto centrado y escalado
        glUseProgram(0);
        glViewport(0, 0, width, height);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, 800, 600, 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glColor3f(0.0f, 0.0f, 0.0f); // Texto negro

        const char* labels[] = { "   JUGAR", "      OPCIONES", "     CREDITOS", "   SALIR" };
        for (int i = 0; i < 4; i++) {
            float centerY = (600.0f / 2.0f) - (yOffsets[i] * 600.0f / 2.0f);
            float centerX = 400.0f;
            drawTextScaledCentered(labels[i], centerX, centerY, 2.5f); // Escala de 2.0
        }

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