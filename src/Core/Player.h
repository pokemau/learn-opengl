#ifndef SLOMMGL_PLAYER_H
#define SLOMMGL_PLAYER_H

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

enum PlayerState { IDLE, WALKING, JUMPING, FALLING };
enum PlayerMoveDir { FORWARD, BACKWARD, LEFT, RIGHT };


class Player {

    const float JUMPVEL = 20.0f;


  public:
    Player(GLFWwindow *window);
    ~Player();

    PlayerState state;
    PlayerMoveDir dir;

    glm::mat4 GetViewMatrix();

    void Update(float deltaTime);
    void ProcessKeyboardInput(GLFWwindow *window, float deltaTime);
    void UpdateCamera();
    void MouseLook(float xOffset, float yOffset);

  private:
    GLFWwindow *_window;
    glm::vec3 _position;
    glm::vec3 _viewPos;
    glm::vec3 _forward;
    glm::vec3 _up;
    glm::vec3 _right;
    glm::vec3 _worldUp;
    float _moveSpeed;
    float _mouseSens;
    float _zoom;
    float Yaw;
    float Pitch;
    

    // TODO: TEST JUMP VARS
    float _jumpSpeed;

    void initCamera();
    void updateCameraVectors();
    void jump();

    bool _isMoving;
    bool _isGrounded;
    // Movement
    bool PressingJump(GLFWwindow *window);
    bool PressingWalkForward(GLFWwindow *window);
    bool PressingWalkBackward(GLFWwindow *window);
    bool PressingWalkRight(GLFWwindow *window);
    bool PressingWalkLeft(GLFWwindow *window);
};

#endif
