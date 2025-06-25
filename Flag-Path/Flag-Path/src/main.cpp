#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <cfloat> 
#include "CollisionBox.h"
// ImGui includes
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <Iniparser.h>
<<<<<<< HEAD

=======
>>>>>>> 2c95cca9a5c699e979625d20fc2189c6d6d49552
float lastX = 400, lastY = 300;
bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 1.0f, 0.0f));

void processInput(GLFWwindow* window, Camera& camera, std::vector<CollisionBox>& colliders, float deltaTime) {
    glm::vec3 direction(0.0f);
    float velocity = camera.Speed * deltaTime;

    // Movimiento en plano XZ relativo al Yaw de la cámara
    glm::vec3 flatFront = glm::normalize(glm::vec3(camera.Front.x, 0.0f, camera.Front.z));
    glm::vec3 flatRight = glm::normalize(glm::vec3(camera.Right.x, 0.0f, camera.Right.z));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        direction += flatFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        direction -= flatFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        direction -= flatRight;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        direction += flatRight;

    if (glm::length(direction) > 0.0f) {
        direction = glm::normalize(direction);
        glm::vec3 nextPos = camera.Position + direction * velocity;

        float camRadius = 1.0f;

        bool colliding = false;
        for (const auto& box : colliders) {
            if (box.collidesWithSphere(nextPos, camRadius)) {
                colliding = true;
                break;
            }
        }

        if (!colliding) {
            camera.Position = nextPos;
        }
    }
}



unsigned int loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}



// Vértices del cubo para el skybox
float skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};
std::vector<glm::vec3> banderaPosiciones = {
    {-1.8f, 1.0f,  3.0f},   // Guatemala  
    {-1.8f, 1.0f,  1.5f},   // Costa Rica
    {-1.8f, 1.0f,  0.0f},   // Nicaragua 
    {-1.8f, 1.0f, -1.5f},   // El Salvador
    {-1.8f, 1.0f, -3.0f}    // Honduras
};

std::vector<std::string> banderaPosicionesNames = {
    "Guatemala", "CostaRica", "Nicaragua", "ElSalvador", "Honduras"
};
std::vector<std::string> banderaNombres = {
    "Guatemala",
    "Costa Rica",
    "Nicaragua",
    "El Salvador",
<<<<<<< HEAD
    "Honduraz"
=======
    "Honduras"
>>>>>>> 2c95cca9a5c699e979625d20fc2189c6d6d49552
};

int getBanderaCercana(const glm::vec3& camPos, const std::vector<glm::vec3>& posiciones, float distancia = 1.5f) {
    for (int i = 0; i < posiciones.size(); ++i) {
        if (glm::distance(camPos, posiciones[i]) <= distancia)
            return i;
    }
<<<<<<< HEAD

=======
>>>>>>> 2c95cca9a5c699e979625d20fc2189c6d6d49552
    return -1;
}



void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = 400, lastY = 300;
    static bool firstMouse = true;
    if (firstMouse) {
        lastX = float(xpos);
        lastY = float(ypos);
        firstMouse = false;
    }
    float xoffset = float(xpos) - lastX;
    float yoffset = lastY - float(ypos); // reversed: y ranges bottom to top
    lastX = float(xpos);
    lastY = float(ypos);
    camera.ProcessMouseMovement(xoffset, yoffset);
}


void ApplyCustomImGuiStyle()
{
    // Base Dark (puedes cambiar a StyleColorsLight o Classic)
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    // Rounding y padding
    style.WindowRounding = 8.0f;
    style.FrameRounding = 4.0f;
    style.PopupRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
<<<<<<< HEAD
=======


 

    // Colores personalizados
    auto& colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.14f, 0.95f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.45f, 0.70f, 0.8f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.55f, 0.85f, 0.9f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.40f, 0.65f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.50f, 0.90f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.60f, 0.95f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.45f, 0.85f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.40f, 0.60f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.12f, 0.30f, 0.50f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.08f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.20f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.20f, 0.50f, 0.90f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.25f, 0.60f, 0.95f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.20f, 0.50f, 0.90f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.45f, 0.70f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.25f, 0.55f, 0.85f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.15f, 0.40f, 0.65f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.28f, 0.45f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.18f, 0.30f, 0.50f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.13f, 0.24f, 0.42f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.12f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.20f, 0.30f, 1.00f);

}
>>>>>>> 2c95cca9a5c699e979625d20fc2189c6d6d49552


 

    // Colores personalizados
    auto& colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.14f, 0.95f);
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.45f, 0.70f, 0.8f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.55f, 0.85f, 0.9f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.40f, 0.65f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.50f, 0.90f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.60f, 0.95f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.45f, 0.85f, 1.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.40f, 0.60f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.12f, 0.30f, 0.50f, 0.67f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.08f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.20f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.20f, 0.50f, 0.90f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.25f, 0.60f, 0.95f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.20f, 0.50f, 0.90f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.45f, 0.70f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.25f, 0.55f, 0.85f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.15f, 0.40f, 0.65f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.28f, 0.45f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.18f, 0.30f, 0.50f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.13f, 0.24f, 0.42f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.12f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.20f, 0.30f, 1.00f);

<<<<<<< HEAD
}

=======
>>>>>>> 2c95cca9a5c699e979625d20fc2189c6d6d49552
int program() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    std::map<std::string, std::string> banderaInfoMap;

    // En program(), antes de ImGui setup:
    if (!LoadBanderaInfo("info.ini", banderaInfoMap)) {
        std::cerr << "¡Error cargando info.ini!\n";
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "En este estado carga modelos.", NULL, NULL);
<<<<<<< HEAD
=======
   //GLFWwindow* window = glfwCreateWindow(1920, 1100, "tamaño de ventana para PC de alvaro.", NULL, NULL);
>>>>>>> 2c95cca9a5c699e979625d20fc2189c6d6d49552
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

<<<<<<< HEAD

=======
>>>>>>> 2c95cca9a5c699e979625d20fc2189c6d6d49552
    // Inicialización de FMOD
    FMOD::System* fmodSystem = nullptr;
    FMOD::Sound* backgroundSound = nullptr;
    FMOD::Channel* channel = nullptr;

    FMOD::System_Create(&fmodSystem);
    fmodSystem->init(512, FMOD_INIT_NORMAL, nullptr);

    // Carga el sonido (asegúrate de que la ruta y el archivo existan)
    fmodSystem->createSound("res/prueba.mp3", FMOD_LOOP_NORMAL | FMOD_2D, nullptr, &backgroundSound);

    // Reproduce el sonido en loop
    fmodSystem->playSound(backgroundSound, nullptr, false, &channel);
    

    // Skybox setup
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Rutas a tus imágenes de skybox
    std::vector<std::string> faces
    {
        "res/Textures/Skybox/right.jpg",
        "res/Textures/Skybox/left.jpg",
        "res/Textures/Skybox/top.jpg",
        "res/Textures/Skybox/bottom.jpg",
        "res/Textures/Skybox/front.jpg",
        "res/Textures/Skybox/back.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    Shader skyboxShader("res/Shaders/skybox.vs", "res/Shaders/skybox.fs");

    glEnable(GL_DEPTH_TEST);

    Shader shader("res/Shaders/shader.vs", "res/Shaders/shader.fg");
<<<<<<< HEAD

=======
>>>>>>> 2c95cca9a5c699e979625d20fc2189c6d6d49552
    Model model("res/Models/Exhibition/FinalDesign.gltf");
    // Model model2("res/Models/vivr/survivalrio_unity_mat.fbx");
    float width = 7.0f;      // ancho total en X
    float depth = 9.0f;      // profundidad total en Z
    float height = 3.0f;      // altura en Y
    float wallThickness = 0.3f; // grosor de las paredes

    std::vector<CollisionBox> colliders = {
        // Frontal (Z+)
        CollisionBox(glm::vec3(0.0f, height / 2, depth / 2 + wallThickness / 2),
                     glm::vec3(width, height, wallThickness)),

                     // Trasera (Z-)
                     CollisionBox(glm::vec3(0.0f, height / 2, -depth / 2 - wallThickness / 2),
                                  glm::vec3(width, height, wallThickness)),

                                  // Derecha (X+)
                                  CollisionBox(glm::vec3(width / 2 + wallThickness / 2, height / 2, 0.0f),
                                               glm::vec3(wallThickness, height, depth)),

                                               // Izquierda (X-)
                                               CollisionBox(glm::vec3(-width / 2 - wallThickness / 2, height / 2, 0.0f),
                                                            glm::vec3(wallThickness, height, depth)),
    };


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ApplyCustomImGuiStyle();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

<<<<<<< HEAD

=======
>>>>>>> 2c95cca9a5c699e979625d20fc2189c6d6d49552

    float lastFrame = 0.0f;
    bool showInfo = false;
    int currentIndex = -1;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

   


        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        glm::mat4 skyboxProjection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        skyboxShader.setMat4("view", skyboxView);
        skyboxShader.setMat4("projection", skyboxProjection);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        skyboxShader.setInt("skybox", 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        // Posición del sol en el mundo (ajusta según tu escena)
        glm::vec3 sunWorldPos = glm::vec3(10.0f, 20.0f, -10.0f);

        // Calcula el modelo para el quad (escalado y posición)
        glm::mat4 sunModel = glm::mat4(1.0f);
        sunModel = glm::translate(sunModel, sunWorldPos);
        sunModel = glm::scale(sunModel, glm::vec3(3.0f)); // tamaño del sol


        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);






        shader.use();


        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(-2.60f, 0.0f, 0.0f)); 
        modelMat = glm::scale(modelMat, glm::vec3(0.1f));
        //modelMat = glm::rotate(modelMat, glm::radians(-90.0f), glm::vec3(1, 0, 0));
       
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
     
        model.Draw(shader, modelMat);
        processInput(window, camera, colliders, deltaTime);
        int bandera = getBanderaCercana(camera.Position, banderaPosiciones);
        if (bandera != -1 && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
            showInfo = true;
            currentIndex = bandera;
        }
        if (showInfo && glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
            showInfo = false;
        }

        // Inicia el frame de ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1) Texto “Presiona E para interactuar” centrado, sólo si estoy cerca y no se está mostrando el panel
        if (bandera != -1 && !showInfo) {
            ImGuiIO& io = ImGui::GetIO();
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration
                | ImGuiWindowFlags_AlwaysAutoResize;
            // Coloca la ventana justo en el centro
            ImGui::SetNextWindowPos(
                ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
                ImGuiCond_Always,
                ImVec2(0.5f, 0.5f)
            );
            ImGui::Begin("##InteractPrompt", nullptr, flags);
            ImGui::Text("Presiona E para interactuar");
            ImGui::End();
        }

        // 2) Panel de detalle que aparece al presionar E
        if (showInfo && currentIndex >= 0) {
            // Obtén el nombre de la bandera actual
            std::string name = banderaPosicionesNames[currentIndex];

            // Crea un título único para ImGui usando el nombre
            std::string windowTitle = "Bandera de " + name;

            ImGui::Begin(windowTitle.c_str(), &showInfo,
                ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoCollapse);

            // Busca en el mapa cargado desde el INI
            auto it = banderaInfoMap.find(name);
            if (it != banderaInfoMap.end()) {
                // Si hay entrada, muestra el texto
                ImGui::TextWrapped("%s", it->second.c_str());

            }
            else {
                // Sino un mensaje de fallback
                ImGui::Text("No hay informacion para %s", name.c_str());
            }

            ImGui::Separator();
            ImGui::Text("Presiona Backspace para cerrar");
            ImGui::End();
        }


<<<<<<< HEAD

=======
>>>>>>> 2c95cca9a5c699e979625d20fc2189c6d6d49552
        // Renderiza ImGui encima de la escena
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Finalmente, intercambia buffers y procesa eventos
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 6) —— Limpieza de ImGui y GLFW al salir —
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
