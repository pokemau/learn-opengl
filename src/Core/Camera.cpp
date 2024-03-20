#include "Camera.h"

#include <iostream>

Camera::Camera()
    : MoveSpeed(SPEED), MouseSens(SENS), JumpSpeed(JUMPVEL), Zoom(ZOOM),
      Position(glm::vec3(0.0f, 0.0f, 10.0f)),
      Front(glm::vec3(0.0f, 0.0f, -1.0f)), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
      Yaw(YAW), Pitch(PITCH), state(IDLE) {

    updateCameraVectors();
}

void Camera::ProcessKeyboardMovement(CameraMovementDirection dir,
                                     float deltaTime) {

    float velocity = MoveSpeed * deltaTime;

    glm::vec3 tempUp = glm::vec3(0.0f, 1.0f, 0.0f);

    if (dir == JUMP) {
        state = JUMPING;
    }

    if (dir == FORWARD) {
        Position += Front * velocity;
    }
    if (dir == BACKWARD) {
        Position += -Front * velocity;
    }
    if (dir == LEFT) {
        Position -= Right * velocity;
    }
    if (dir == RIGHT) {
        Position += Right * velocity;
    }

    if (state != JUMPING && state != GOING_DOWN) {
        Position.y = 0.0f;
    }

}

void Camera::Draw(float deltaTime) {

    float velocity = upSpeed * deltaTime;

    float maxJumpHeigt = 2.5f;

    std::cout << "UPSPEED: " << upSpeed << std::endl;

    if (state == JUMPING) {

        upSpeed -= 0.03f;

        Position += WorldUp * velocity;

        if (Position.y >= maxJumpHeigt || upSpeed <= 0.0f) {
            state = GOING_DOWN;
        }
    }

    if (state == GOING_DOWN) {

        upSpeed += 0.03f;

        Position -= WorldUp * velocity;

        if (Position.y <= 0.0f) {
            Position.y = 0.0f;
            upSpeed = JumpSpeed;
            state = IDLE;
        }
    }

    // std::cout << "POS Y: " << Position.y << std::endl;
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
