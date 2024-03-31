#include "Camera.h"

Camera::Camera()
    : MoveSpeed(SPEED), MouseSens(SENS), Zoom(ZOOM),
      Position(glm::vec3(0.0f, 0.0f, 10.0f)),
      Front(glm::vec3(0.0f, 0.0f, -1.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
      Yaw(YAW), Pitch(PITCH) {

    updateCameraVectors();
}


void Camera::ProcessMouseMovement(float xOffset, float yOffset) {

    xOffset *= MouseSens;
    yOffset *= MouseSens;

    Yaw += xOffset;
    Pitch += yOffset;

    // view up and down 90 degrees
    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraVectors() {

    glm::vec3 newFront;
    newFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    newFront.y = sin(glm::radians(Pitch));
    newFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    Front = glm::normalize(newFront);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}
