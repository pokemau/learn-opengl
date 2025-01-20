#include "player.hpp"
#include "GLFW/glfw3.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

Player::Player(GLFWwindow *window) {
    _window = window;

    initCamera();
    updateCameraVectors();

    _jumpSpeed = JUMPVEL;
    _isGrounded = true;
    state = IDLE;
}

Player::~Player() {}

void Player::Update(float deltaTime) {
    ProcessKeyboardInput(_window, deltaTime);

    float jumpVel = _jumpSpeed * deltaTime;

    float maxJumpHeight = 3.0f;

    //     std::cout << "STATE: " << state << std::endl;
    //    std::cout << "Position: " << glm::to_string(_position) << std::endl;
    //    std::cout << "WorldUp: " << glm::to_string(_worldUp) << std::endl;

    // std::cout << "jumpVel : " << jumpVel << std::endl;
    //    std::cout << "Front: " << glm::to_string(_forward)<< std::endl;
    //
    //    AMBOT
    std::cout << "Pos  : " << glm::to_string(_position) << std::endl;
    std::cout << "Front: " << glm::to_string(_forward) << std::endl;

    if (state == JUMPING) {
        _position += _worldUp * jumpVel;
        _isGrounded = false;
        _jumpSpeed -= 0.02f;

        if (_jumpSpeed <= 0.0f) {
            state = FALLING;
        }

    } else if (state == FALLING) {

        _position -= _worldUp * jumpVel;
        _jumpSpeed += 0.02f;

        if (_position.y <= 0.0f) {
            _position.y = 0.0f;
            _jumpSpeed = JUMPVEL;
            state = IDLE;
            _isGrounded = true;
        }
    }
}

void Player::ProcessKeyboardInput(GLFWwindow *window, float deltaTime) {

    glm::vec3 tempFront =
        glm::normalize(glm::vec3(_forward.x, 0.0f, _forward.z));
    float velocity = _moveSpeed * deltaTime;

    _isMoving = false;
    if (PressingWalkForward(window)) {
        _position += tempFront * velocity;
        _isMoving = true;
    }
    if (PressingWalkLeft(window)) {
        _position -= _right * velocity;
        _isMoving = true;
    }
    if (PressingWalkBackward(window)) {
        _position -= tempFront * velocity;
        _isMoving = true;
    }
    if (PressingWalkRight(window)) {
        _position += _right * velocity;
        _isMoving = true;
    }
    if (PressingJump(window) && _isGrounded == true && state != JUMPING) {
        state = JUMPING;
    }
}

bool Player::PressingJump(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        return true;
    } else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        return false;
    }
    return false;
}

bool Player::PressingWalkForward(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        return true;
    } else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
        return false;
    }
    return false;
}
bool Player::PressingWalkBackward(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        return true;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
        return false;
    }
    return false;
}
bool Player::PressingWalkRight(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        return true;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        return false;
    }
    return false;
}
bool Player::PressingWalkLeft(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        return true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
        return false;
    }
    return false;
}

glm::mat4 Player::GetViewMatrix() {
    return glm::lookAt(_position, _position + _forward, _up);
}

void Player::MouseLook(float xOffset, float yOffset) {
    xOffset *= _mouseSens;
    yOffset *= _mouseSens;
    Yaw += xOffset;
    Pitch += yOffset;

    // view up and down 90 degrees
    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;

    updateCameraVectors();
}

void Player::initCamera() {
    Yaw = -90.0f;
    Pitch = 0.0f;

    _moveSpeed = 8.0f;
    _mouseSens = 0.07f;
    _zoom = 45.0f;
    _position = glm::vec3(0.0f, 0.0f, 10.0f);
    _forward = glm::vec3(0.0f, 0.0f, -1.0f);
    _worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
}
void Player::updateCameraVectors() {

    glm::vec3 newFront;
    newFront.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    newFront.y = sin(glm::radians(Pitch));
    newFront.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    _forward = glm::normalize(newFront);
    _right = glm::normalize(glm::cross(_forward, _worldUp));
    _up = glm::normalize(glm::cross(_right, _forward));
}
