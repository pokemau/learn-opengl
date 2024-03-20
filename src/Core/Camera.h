#ifndef SLOMMGL_CAMERA_H
#define SLOMMGL_CAMERA_H

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

enum CameraMovementDirection {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    JUMP,
    DOWN,
    RESET
};

enum PlayerState {
    IDLE,
    WALKING,
    JUMPING,
    GOING_DOWN,
};

const float YAW     = -90.0f;
const float PITCH   = 0.0f;
const float SPEED   = 7.0f;
const float SENS    = 0.07f;
const float ZOOM    = 45.0f;
const float JUMPVEL = 10.0f;

class Camera {

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MoveSpeed;
    float MouseSens;
    float JumpSpeed;
    float Zoom;

    // TODO
    // test jump vars
    float upSpeed = JumpSpeed;

public:
    PlayerState state;

    Camera();
    glm::mat4 GetViewMatrix();

    void Draw(float deltaTime);
    void ProcessMouseMovement(float xOffset, float yOffset);
    void ProcessKeyboardMovement(CameraMovementDirection dir, float deltaTime);

private:
    void updateCameraVectors();
};


#endif //SLOMMGL_CAMERA_H
