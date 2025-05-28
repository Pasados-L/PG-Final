#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <FMOD/fmod.hpp>


int main() {
    FMOD::System* system = nullptr;
    FMOD::Sound* sound = nullptr;
    FMOD::Channel* channel = nullptr;

    // Inicializa FMOD
    FMOD::System_Create(&system);
    system->init(512, FMOD_INIT_NORMAL, nullptr);

    // Carga el sonido
    system->createSound("test.wav", FMOD_DEFAULT, nullptr, &sound);

    // Reproduce el sonido
    system->playSound(sound, nullptr, false, &channel);

    // Espera hasta que termine el sonido
    bool isPlaying = true;
    while (isPlaying) {
        system->update();
        channel->isPlaying(&isPlaying);
    }

    // Libera recursos
    sound->release();
    system->close();
    system->release();

    return 0;
}

