#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCREEN_WIDTH = 1200;
const unsigned int SCREEN_HEIGHT = 800;


//shaders
const char* vertexShaderSource = R"glsl(
#version 430 core

layout (location = 0) in vec3 Pos;
layout (location = 2) uniform mat4 u_TranslateMatrix;

void main()
{
   gl_Position = u_TranslateMatrix * vec4(Pos, 1.0);
}
)glsl";

const char* geometryShaderSource = R"glsl(
#version 430 core

layout (points) in;
layout (line_strip, max_vertices = 100) out;

layout (location = 3) uniform mat4 u_ProjMatrix;

void draw_circle(vec3 center, float radius, mat4 projection, int divisions)
{
    for (float i=0; i<divisions+1; i++)
    {
        vec4 point_on_circle;
        point_on_circle.x = center.x + radius * cos(i * radians(360)/divisions);
        point_on_circle.y = center.y + radius * sin(i * radians(360)/divisions);
        point_on_circle.z = center.z;
        point_on_circle.w = 1.0;
    
        gl_Position = projection * point_on_circle;
        EmitVertex();
    }

    EndPrimitive();
}


void main()
{
    vec3 center = gl_in[0].gl_Position.xyz;
    draw_circle(center, 0.3, u_ProjMatrix, 12);
}
)glsl";

const char* fragmentShaderSource = R"glsl(
#version 430 core

layout (location = 1) uniform vec4 TriangleColor;

out vec4 FragColor;
void main()
{
   FragColor = TriangleColor;
}
)glsl";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL Przeksztalcenia", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // 
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // geometry shader
    unsigned int geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
    glCompileShader(geometryShader);

    glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, geometryShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);


    static const float My_Points[] = {
            0.0f,  0.0f, 0.0f,
            1.0f,  0.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
            1.0f,  1.0f, 0.0f
    };


    unsigned int VertexBufferId, VertexArrayId;
    glGenVertexArrays(1, &VertexArrayId);
    glGenBuffers(1, &VertexBufferId);
    glBindVertexArray(VertexArrayId);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(My_Points), &My_Points, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glfwSetTime(0);
    glfwSwapInterval(1);

    //game loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.6f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VertexArrayId);

        float CurrentTime = glfwGetTime();

        glm::mat4 TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, -7.0f));
        glm::mat4 ProjMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.01f, 100.0f);

        glUniformMatrix4fv(2, 1, GL_FALSE, glm::value_ptr(TranslateMatrix));
        glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(ProjMatrix));

        glUniform4f(1, 0.0f, 0.0f, 1.0f, 1.0f);
        glDrawArrays(GL_POINTS, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VertexArrayId);
    glDeleteBuffers(1, &VertexBufferId);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}