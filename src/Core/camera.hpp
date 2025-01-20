#ifndef SLOMMGL_CAMERA_H
#define SLOMMGL_CAMERA_H

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 7.0f;
const float SENS = 0.07f;
const float ZOOM = 45.0f;

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
    float Zoom;

  public:
    Camera();
    glm::mat4 GetViewMatrix();
    void ProcessMouseMovement(float xOffset, float yOffset);

  private:
    void updateCameraVectors();
};

#endif // SLOMMGL_CAMERA_H
