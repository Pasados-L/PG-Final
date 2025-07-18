#include "Camera.h"
#include <GLFW/glfw3.h>
#include <cmath>

Camera::Camera(glm::vec3 position)
    : Position(position), WorldUp(0.0f, 1.0f, 0.0f), Yaw(-90.0f), Pitch(0.0f), Speed(7.5f), MouseSensitivity(0.12f)
{
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(int key, float deltaTime) {
    float velocity = Speed * deltaTime;

    // Movimiento como en FPS: en el plano XZ, seg�n la direcci�n de vista (Front)
    glm::vec3 flatFront = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
    glm::vec3 flatRight = glm::normalize(glm::vec3(Right.x, 0.0f, Right.z));

    if (key == GLFW_KEY_W)
        Position += flatFront * velocity;
    if (key == GLFW_KEY_S)
        Position -= flatFront * velocity;
    if (key == GLFW_KEY_A)
        Position -= flatRight * velocity;
    if (key == GLFW_KEY_D)
        Position += flatRight * velocity;
}


void Camera::ProcessMouseMovement(float xoffset, float yoffset) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}


