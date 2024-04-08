#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../headers/Shader_s.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void updatePosition();

const unsigned int SCREEN_WIDTH = 1200;
const unsigned int SCREEN_HEIGHT = 800;

float rot_angle = 0.0f;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

float v = 1.f;
float s = 4.f;
float c = 0.45f;
float gravity = 9.8f;

void checkCollision();

bool shouldMove = true;

void updatePosition() {

    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (!shouldMove) {
        return;
    }
    s += (v * deltaTime) + (0.5f * gravity * deltaTime * deltaTime);
    v += -gravity * deltaTime;

    checkCollision();
}

void checkCollision() {
    if (s < 0 && v < 0) {
        v = -c * v;
        s = -s;
        if (abs(v) < (0.5f)) {
            v = 0;
            s = 0;
            shouldMove = false;
            return;
        }
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Przeksztalcenia", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);





    // build and compile our shader program
    // vertex shader

    Shader shader("resources/shaders/rotacje/vertex.vs", "resources/shaders/rotacje/fragment.fs");

    static const float Qube[] = {
            -0.5f, -0.5f, -0.5f, //bottom
            0.5f, -0.5f, -0.5f,
            0.5f, 0.5f, -0.5f,
            -0.5f, 0.5f, -0.5f,

            -0.5f, -0.5f, 0.5f, //top
            0.5f, -0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,

            -20.0f, -0.5f, 50.0f, //ground
            -20.0f, -0.5f, -50.0f,
            20.0f, -0.5f, -50.0f,
            20.0f, -0.5f, 50.0f
    };

    unsigned int Indices[] = {
            0, 1, 2, //bottom
            2, 3, 0,
            4, 5, 6, //top
            6, 7, 4,
            0, 1, 5, //first side
            5, 4, 0,
            1, 2, 6, //second side
            6, 5, 1,
            2, 3, 7, //third side
            7, 6, 2,
            3, 0, 4, //fourth side
            4, 7, 3,

            8, 9, 10, //ground
            10, 11, 8
    };

    unsigned int VertexBufferId, VertexArrayId, ElementBufferId;
    glGenVertexArrays(1, &VertexArrayId);
    glGenBuffers(1, &VertexBufferId);
    glGenBuffers(1, &ElementBufferId);
    glBindVertexArray(VertexArrayId);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Qube), Qube, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    glfwSetTime(0);
    glfwSwapInterval(1);

    //game loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VertexArrayId);

        //macierz rzutowania

        glm::mat4 ProjMatrix = glm::perspective(glm::radians(45.0f), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT,
                                                0.01f,
                                                100.0f);
        glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(ProjMatrix));

        //macierz widoku

        glm::mat4 ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -10.0f));
        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
        glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

        //pod³oga

        glUniform4f(1, 0.3f, 0.6f, 0.3f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *) (36 * sizeof(unsigned int)));

        //szeœcian

        ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, s, 0.f));
        ModelMatrix = glm::rotate(ModelMatrix, rot_angle, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

        updatePosition();

        glUniform4f(1, 0.0f, 0.0f, 1.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VertexArrayId);
    glDeleteBuffers(1, &VertexBufferId);
    glDeleteBuffers(1, &ElementBufferId);
    glDeleteProgram(shader.ID);

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        shouldMove = true;
        v = 5;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        c += 0.05f;
        std::cout << "c: " << c << std::endl;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        c -= 0.05f;
        std::cout << "c: " << c << std::endl;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
