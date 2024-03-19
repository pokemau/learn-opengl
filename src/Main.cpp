#include "Core/Camera.h"
#include "Core/GL.h"
#include "Core/Shader.h"
#include "Core/Texture.h"

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

void mouse_callback(GLFWwindow *window, double xPosIN, double yPosIN);
void processInput(GLFWwindow *window);
unsigned int loadCubemap(std::vector<std::string> faces);

const int SCREEN_W = 800;
const int SCREEN_H = 600;

// camera
Camera camera{};

float lastX = SCREEN_W / 2.0f;
float lastY = SCREEN_H / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int generateTerrain();
unsigned int drawCubes();
unsigned int drawSkybox();
unsigned int drawWall();

double lastTime = glfwGetTime();
int nbFrames = 0;

int main() {
  GL app{};

  app.Init(SCREEN_W, SCREEN_H, "Fake");
  glEnable(GL_DEPTH_TEST);

  GLFWwindow *win = app.GetWindowPointer();

  glfwSwapInterval(0);
  glfwSetCursorPosCallback(win, mouse_callback);
  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  Shader shader("../res/shaders/vertex.vert",
                              "../res/shaders/fragment.frag");

  Shader skyboxShader("../res/shaders/skybox/skybox_Vertex.vert",
                      "../res/shaders/skybox/skybox_Fragment.frag");

  unsigned int T_VAO = generateTerrain();
  unsigned int VAO = drawCubes();
  unsigned int skyboxVAO = drawSkybox();
  unsigned int wallVAO = drawWall();

  int terrainPosLen = 9;
  glm::vec3 terrainPos[] = {
      glm::vec3(-20.0f, 0.0f, 10.0f),  glm::vec3(0.0f, 0.0f, 10.0f),
      glm::vec3(20.0f, 0.0f, 10.0f),   glm::vec3(-20.0f, 0.0f, -10.0f),
      glm::vec3(0.0f, 0.0f, -10.0f),   glm::vec3(20.0f, 0.0f, -10.0f),
      glm::vec3(-20.0f, 0.0f, -30.0f), glm::vec3(0.0f, 0.0f, -30.0f),
      glm::vec3(20.0f, 0.0f, -30.0f),
  };
  glm::vec3 wallPos[] = {
      glm::vec3(-20.0f, 0.0f, 10.0f),  glm::vec3(0.0f, 0.0f, 10.0f),
      glm::vec3(20.0f, 0.0f, 10.0f),   glm::vec3(-20.0f, 0.0f, -10.0f),
      glm::vec3(0.0f, 0.0f, -10.0f),   glm::vec3(20.0f, 0.0f, -10.0f),
      glm::vec3(-20.0f, 0.0f, -30.0f), glm::vec3(0.0f, 0.0f, -30.0f),
      glm::vec3(20.0f, 0.0f, -30.0f),
  };
  glm::vec3 cubePositions[] = {
      glm::vec3(0.0f, 7.0f, 0.0f),
      glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, 2.2f, -2.5f),
  };
  std::vector<std::string> cubemapFaces{"right.jpg",  "left.jpg",  "top.jpg",
                                        "bottom.jpg", "front.jpg", "back.jpg"};

  unsigned int cubemapTexture = loadCubemap(cubemapFaces);

  Texture natTexture = Texture("../res/textures/room.jpg", JPG, GL_TEXTURE_2D);
  Texture floorTexture = Texture("../res/textures/floor.jpg", JPG, GL_TEXTURE_2D);
  Texture wallTexture =
      Texture("../res/textures/brick_wall.jpg", JPG, GL_TEXTURE_2D);

  shader.use();
  shader.setInt("tex1", 0);

  skyboxShader.use();
  skyboxShader.setInt("skybox", 0);

  while (app.WindowIsOpen()) {

    auto currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    camera.Draw(deltaTime);
    app.ProcessInput();

    processInput(win);

    glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, natTexture.GetID());

    glm::mat4 projection = glm::perspective(
                                glm::radians(45.0f), 
                                (float)SCREEN_W / (float)SCREEN_H, 
                                0.1f, 100.0f);

    glm::mat4 view = camera.GetViewMatrix();

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    // draw cubes
    for (int i = 0; i < 3; i++) {

      // model matrix
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float rotation = (float)glfwGetTime() + (50 * (float)i);
      model = glm::rotate(model, rotation, glm::vec3(0.5f, 1.0f, 1.0f));
      shader.setMat4("model", model);

      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // terrain
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexture.GetID());

    for (int i = 0; i < terrainPosLen; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, terrainPos[i]);

      shader.setMat4("model", model);

      glBindVertexArray(T_VAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // wall
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wallTexture.GetID());

    for (int i = 0; i < 3; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, wallPos[i]);
      shader.setMat4("model", model);
      glBindVertexArray(wallVAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // skybox
    glBindVertexArray(0);

    // draw skybox as last
    glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when
                            // values are equal to depth buffer's content
    skyboxShader.use();
    view = glm::mat4(glm::mat3(
        camera.GetViewMatrix())); // remove translation from the view matrix
    skyboxShader.setMat4("view", view);
    skyboxShader.setMat4("projection", projection);
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default

    app.SwapBuffersPollEvents();
  }

  glDeleteVertexArrays(1, &T_VAO);
  glDeleteVertexArrays(1, &VAO);
  glDeleteVertexArrays(1, &skyboxVAO);
  glDeleteVertexArrays(1, &wallVAO);
  //    glDeleteBuffers(1, &T_VBO);
  //    glDeleteBuffers(1, &VBO);

  GL::Cleanup();

  glfwTerminate();
  return 0;
}
void mouse_callback(GLFWwindow *window, double xPosIN, double yPosIN) {
  auto xPos = static_cast<float>(xPosIN);
  auto yPos = static_cast<float>(yPosIN);

  if (firstMouse) {
    lastX = xPos;
    lastY = yPos;
    firstMouse = false;
  }

  float xOffset = xPos - lastX;
  float yOffset = lastY - yPos;

  lastX = xPos;
  lastY = yPos;

  camera.ProcessMouseMovement(xOffset, yOffset);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

  // movement
  /*
  if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    camera.ProcessKeyboardMovement(RESET, deltaTime);
  */

    if (camera.state == IDLE || camera.state == WALKING) {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            std::cout << camera.state << std::endl;
            camera.ProcessKeyboardMovement(JUMP, deltaTime);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboardMovement(RIGHT, deltaTime);
}

unsigned int loadCubemap(std::vector<std::string> faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++) {
    std::string f = "../res/textures/skybox/" + faces[i];
    unsigned char *data = stbi_load(f.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                   0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      std::cout << "Cubemap tex failed to load at path: " << faces[i]
                << std::endl;
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
unsigned int generateTerrain() {

  unsigned int T_VAO, T_VBO;

  float terrainVertices[] = {
      -10.0f, -2.0f, 10.0f,  0.0f, 0.0f, 10.0f,  -2.0f, 10.0f,  1.0f, 0.0f,
      10.0f,  -2.0f, -10.0f, 1.0f, 1.0f, 10.0f,  -2.0f, -10.0f, 1.0f, 1.0f,
      -10.0f, -2.0f, -10.0f, 0.0f, 1.0f, -10.0f, -2.0f, 10.0f,  0.0f, 0.0f,
  };

  glGenVertexArrays(1, &T_VAO);
  glGenBuffers(1, &T_VBO);
  glBindVertexArray(T_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, T_VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(terrainVertices), terrainVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  return T_VAO;
}
unsigned int drawCubes() {

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bot left
      0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, // bot right
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, // top right
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, // top right
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, // top left
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bot left

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // bot left
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, // bot right
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top right
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top right
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, // top left
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // bot left

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // bot left
      -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bot right
      -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, // top right
      -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, // top right
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, // top left
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // bot left

      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, // bot left
      0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, // bot right
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, // top right
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, // top right
      0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top left
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, // bot left

      -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, // bot left
      0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, // bot right
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top right
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top right
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, // top left
      -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, // bot left

      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bot left
      0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, // bot right
      0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, // top right
      0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, // top right
      -0.5f, -0.5f, 0.5f,  0.0f, 1.0f, // top left
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bot left
  };
  unsigned int VAO, VBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  return VAO;
}
unsigned int drawSkybox() {
  float skyboxVertices[] = {
      // positions
      -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
      -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

      1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

      -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

  // skybox VAO
  unsigned int skyboxVAO, skyboxVBO;
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  return skyboxVAO;
}

unsigned int drawWall() {

  glm::vec3 terrainPos[] = {
      glm::vec3(-20.0f, 0.0f, 10.0f),  glm::vec3(0.0f, 0.0f, 10.0f),
      glm::vec3(20.0f, 0.0f, 10.0f),   glm::vec3(-20.0f, 0.0f, -10.0f),
      glm::vec3(0.0f, 0.0f, -10.0f),   glm::vec3(20.0f, 0.0f, -10.0f),
      glm::vec3(-20.0f, 0.0f, -30.0f), glm::vec3(0.0f, 0.0f, -30.0f),
      glm::vec3(20.0f, 0.0f, -30.0f),
  };
  float terrainVertices[] = {
      -10.0f, -2.0f, 10.0f,  0.0f, 0.0f, 10.0f,  -2.0f, 10.0f,  1.0f, 0.0f,
      10.0f,  -2.0f, -10.0f, 1.0f, 1.0f, 10.0f,  -2.0f, -10.0f, 1.0f, 1.0f,
      -10.0f, -2.0f, -10.0f, 0.0f, 1.0f, -10.0f, -2.0f, 10.0f,  0.0f, 0.0f,
  };

  float wallVertices[] = {
      -10.0f, -2.0f, 10.0f, 0.0f, 0.0f, // bot left
      10.0f,  -2.0f, 10.0f, 1.0f, 0.0f, // bot right
      10.0f,  8.0f,  10.0f, 1.0f, 1.0f, // top right

      -10.0f, 8.0f,  10.0f, 0.0f, 1.0f, // top left
      -10.0f, -2.0f, 10.0f, 0.0f, 0.0f, // bot left
      10.0f,  8.0f,  10.0f, 1.0f, 1.0f, // top right
  };

  unsigned int VAO, VBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  return VAO;
}
