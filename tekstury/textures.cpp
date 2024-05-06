#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include "../headers/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "PlayerClass.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

const unsigned int SCREEN_WIDTH = 1200;
const unsigned int SCREEN_HEIGHT = 800;

//global variables
Player Me;
Player enemy;
float rot_angle = glm::radians(90.0f);
float g = -9.98f;
float currentY = 0.0f;

//shaders
const char *vertexShaderSource = R"glsl(
#version 430 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 UVPos;
layout (location = 2) uniform mat4 u_ProjMatrix;
layout (location = 3) uniform mat4 u_ViewMatrix;
layout (location = 4) uniform mat4 u_ModelMatrix;

out vec2 UV;

void main()
{
   gl_Position = u_ProjMatrix * u_ViewMatrix * u_ModelMatrix * vec4(Pos, 1.0);
   UV = UVPos;
}
)glsl";

const char *fragmentShaderSource = R"glsl(
#version 430 core

out vec4 FragColor;
in vec2 UV;

uniform sampler2D TextureSampler;
layout (location = 6) uniform vec4 CubeColor;

void main()
{
    FragColor = texture(TextureSampler, UV) * CubeColor;
}
)glsl";

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
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    static const float Objects[] = {
            //ground
            -10.0f, -0.5f, -10.0f, 0.0f, 0.0f,
            10.0f, -0.5f, -10.0f, 10.0f, 0.0f,
            10.0f, -0.5f, 10.0f, 10.0f, 10.0f,

            10.0f, -0.5f, 10.0f, 10.0f, 10.0f,
            -10.0f, -0.5f, 10.0f, 0.0f, 10.0f,
            -10.0f, -0.5f, -10.0f, 0.0f, 0.0f,

            //cube
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //bottom
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 1.0f,

            0.5f, -0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, //top
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //front
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,

            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, //back
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //left
            -0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, //right
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    };

    //VAO and VBO
    unsigned int VertexBufferId, VertexArrayId;
    glGenVertexArrays(1, &VertexArrayId);
    glGenBuffers(1, &VertexBufferId);
    glBindVertexArray(VertexArrayId);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Objects), Objects, GL_STATIC_DRAW);

    //position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    //texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //load and create a first texture
    unsigned int textureGrassId;

    glGenTextures(1, &textureGrassId);
    glBindTexture(GL_TEXTURE_2D, textureGrassId);
    //set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //load image, create texture and generate mipmaps
    int tex_width, tex_height, tex_nr_Channels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("resources/grass.jpg", &tex_width, &tex_height, &tex_nr_Channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);



    //load and create a second texture
    unsigned int textureCubeId;

    glGenTextures(1, &textureCubeId);
    glBindTexture(GL_TEXTURE_2D, textureCubeId);
    //set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load("resources/wood.jpg", &tex_width, &tex_height, &tex_nr_Channels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    //constant refreshing
    glfwSwapInterval(1);

    enemy.position += glm::vec3(5, 0, 0);
    enemy.speed *= 0.5f;

    bool gameOver = false;
    int score = 0;
    //game loop
    float deltaTime;
    float lastFrame;
    bool alreadyShown = false;
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //choose a background color
        glClearColor(0.7f, 0.7f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        //projection matrix
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(45.0f), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT,
                                                0.01f, 100.0f);
        glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(ProjMatrix));

        //view matrix
        glm::vec3 camera_destination = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 camera_position = glm::vec3(0.0f, 3.0f, 8.0f);
        glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);;

        glm::mat4 ViewMatrix = glm::lookAt(camera_position, camera_destination, camera_up);

        //model matrix
        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
        glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(ModelMatrix));

        //Ground
        glBindTexture(GL_TEXTURE_2D, textureGrassId);
        glUseProgram(shaderProgram);
        glBindVertexArray(VertexArrayId);

        //don't add color to grass
        if (!gameOver)
            glUniform4f(6, 1.0f, 1.0f, 1.0f, 1.0f);
        else {
            glUniform4f(6, 1.0f, 0.0f, 0.0f, 1.0f);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            if (!alreadyShown) {
                std::cout << "Game Over!\n Your score: " << score << std::endl;
                alreadyShown = true;
            }
        }
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //modify model matrix
        if (!gameOver) {
            Me.CheckJump((float) glfwGetTime(), g, currentY);

            ModelMatrix = glm::translate(glm::mat4(1.0f), Me.position);
            ModelMatrix = glm::rotate(ModelMatrix, rot_angle, glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
        }

        //cube
        glBindTexture(GL_TEXTURE_2D, textureCubeId);
        glUseProgram(shaderProgram);

        //make cube red
        glUniform4f(6, 1.0f, 1.0f, 0.0f, 0.0f);

        glDrawArrays(GL_TRIANGLES, 6, 36);

        if (!gameOver) {
            enemy.Move(glm::vec3(-enemy.speed, 0.f, 0.f));
        }
        ModelMatrix = glm::translate(glm::mat4(1.0f), enemy.position);
        glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
        glUniform4f(6, 0.0f, 0.0f, 1.0f, 0.0f);
        glDrawArrays(GL_TRIANGLES, 6, 36);

        // second cube
        if (enemy.position.x <= -6) {
            enemy.position.x = 7;
            score++;
        }
        if (enemy.CheckCollision(Me.boundingBoxMin, Me.boundingBoxMax)) {
            gameOver = true;
        } else {
            gameOver = false;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //clear memory
    glDeleteVertexArrays(1, &VertexArrayId);
    glDeleteBuffers(1, &VertexBufferId);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rot_angle += glm::radians(1.0f);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rot_angle -= glm::radians(1.0f);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        Me.Move(glm::vec3(Me.speed * glm::cos(rot_angle), 0.0f, -Me.speed * glm::sin(rot_angle)));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !(Me.jump_active)) {
        glfwSetTime(0);
        Me.jump_active = true;
        currentY = Me.position.y;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
