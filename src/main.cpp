/*
Mate Barabas 40285661
Rostislav Rusev 40208933
Nicolae Rusu 40245233
*/
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, glm::mat4 &transform);

// Window Size
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;
const float TRANSLATION_DISTANCE = 0.1f;
const float ROTATION_ANGLE = 30.0f;
const float SCALING_FACTOR = 1.1f;

const char *vertexShaderSource = R"glsl(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 transform;
        void main() {
            gl_Position = transform * vec4(aPos, 1.0);
        }
    )glsl";

const char *fragmentShaderSource = R"glsl(
        #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }
    )glsl";

int main()
{
    glfwInit();

    // GLFW Window
    GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "COMP371_ASSIGNMENT2", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Link fragment and vertex shader
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Delete unneeded shader objects after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Triangle Vertex Data
    float triangle[] = {
        -0.5f, -0.5f, 0.0f, // Bottom-Left
        0.5f, -0.5f, 0.0f,  // Bottom-Right
        0.0f, 0.5f, 0.0f,   // Top-Center
    };

    // Setup VAO, VBO, EBO
    unsigned int VBOs[1], VAOs[1], EBO;
    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, VBOs);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAOs[0]);

    // Bind the VBO to the current OpenGL context
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);

    // Copy the vertex data to the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    // Specify how the vertex data should be interpreted
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    // Enable the vertex attribute at location 0
    glEnableVertexAttribArray(0);

    glm::mat4 transform = glm::mat4(1.0f);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window, transform);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // Set the transform uniform
        GLint transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Poll IO events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Deallocate resources
    glDeleteVertexArrays(1, VAOs);
    glDeleteBuffers(1, VBOs);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, glm::mat4 &transform)
{
    static glm::vec3 position(0.0f);
    static float rotation = 0.0f;
    static float scale = 1.0f;

    // Close window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Translation
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position.y += TRANSLATION_DISTANCE;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position.y -= TRANSLATION_DISTANCE;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position.x -= TRANSLATION_DISTANCE;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position.x += TRANSLATION_DISTANCE;

    // Rotation
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        rotation += ROTATION_ANGLE;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        rotation -= ROTATION_ANGLE;

    // Scaling
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        scale *= SCALING_FACTOR;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        scale /= SCALING_FACTOR;

    // Reset to initial values
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        position = glm::vec3(0.0f);
        rotation = 0.0f;
        scale = 1.0f;
    };

    transform = glm::mat4(1.0f);
    transform = glm::translate(transform, position);
    transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(scale, scale, scale));
}

// Resize window callback
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // Match viewport to new window dimensions
    glViewport(0, 0, width, height);
}
