// Lab06.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Lab06.cpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


// ----------------------------------------------------
// Function declarations
// ----------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


// ----------------------------------------------------
// Read Shader File
// ----------------------------------------------------
std::string readShaderFile(const char* filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cout << "ERROR: Cannot open shader file: "
            << filePath << std::endl;

        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    file.close();

    return buffer.str();
}


// ----------------------------------------------------
// Create Shader Program
// ----------------------------------------------------
unsigned int createShaderProgram(
    const char* vertexPath,
    const char* fragmentPath)
{
    std::string vertexCode = readShaderFile(vertexPath);
    std::string fragmentCode = readShaderFile(fragmentPath);

    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    // Vertex Shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(
        vertexShader,
        1,
        &vertexShaderSource,
        NULL
    );

    glCompileShader(vertexShader);

    // Check Vertex Shader
    int success;
    char infoLog[512];

    glGetShaderiv(
        vertexShader,
        GL_COMPILE_STATUS,
        &success
    );

    if (!success)
    {
        glGetShaderInfoLog(
            vertexShader,
            512,
            NULL,
            infoLog
        );

        std::cout
            << "ERROR::VERTEX SHADER COMPILATION\n"
            << infoLog
            << std::endl;
    }


    // Fragment Shader
    unsigned int fragmentShader;

    fragmentShader = glCreateShader(
        GL_FRAGMENT_SHADER
    );

    glShaderSource(
        fragmentShader,
        1,
        &fragmentShaderSource,
        NULL
    );

    glCompileShader(fragmentShader);

    // Check Fragment Shader
    glGetShaderiv(
        fragmentShader,
        GL_COMPILE_STATUS,
        &success
    );

    if (!success)
    {
        glGetShaderInfoLog(
            fragmentShader,
            512,
            NULL,
            infoLog
        );

        std::cout
            << "ERROR::FRAGMENT SHADER COMPILATION\n"
            << infoLog
            << std::endl;
    }


    // Shader Program
    unsigned int shaderProgram;

    shaderProgram = glCreateProgram();

    glAttachShader(
        shaderProgram,
        vertexShader
    );

    glAttachShader(
        shaderProgram,
        fragmentShader
    );

    glLinkProgram(shaderProgram);

    // Check Shader Program
    glGetProgramiv(
        shaderProgram,
        GL_LINK_STATUS,
        &success
    );

    if (!success)
    {
        glGetProgramInfoLog(
            shaderProgram,
            512,
            NULL,
            infoLog
        );

        std::cout
            << "ERROR::SHADER PROGRAM LINKING\n"
            << infoLog
            << std::endl;
    }


    // Delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


// ----------------------------------------------------
// Settings
// ----------------------------------------------------
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


// ----------------------------------------------------
// Main
// ----------------------------------------------------
int main()
{
    // ------------------------------------------------
    // GLFW initialize
    // ------------------------------------------------

    glfwInit();

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MAJOR,
        3
    );

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MINOR,
        3
    );

    glfwWindowHint(
        GLFW_OPENGL_PROFILE,
        GLFW_OPENGL_CORE_PROFILE
    );


    // ------------------------------------------------
    // Create Window
    // ------------------------------------------------

    GLFWwindow* window =
        glfwCreateWindow(
            SCR_WIDTH,
            SCR_HEIGHT,
            "Lab06",
            NULL,
            NULL
        );

    if (window == NULL)
    {
        std::cout
            << "Failed to create GLFW window"
            << std::endl;

        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(
        window,
        framebuffer_size_callback
    );


    // ------------------------------------------------
    // Initialize GLAD
    // ------------------------------------------------

    if (!gladLoadGLLoader(
        (GLADloadproc)glfwGetProcAddress))
    {
        std::cout
            << "Failed to initialize GLAD"
            << std::endl;

        return -1;
    }


    // ------------------------------------------------
    // Viewport
    // ------------------------------------------------

    glViewport(
        0,
        0,
        SCR_WIDTH,
        SCR_HEIGHT
    );


    // ------------------------------------------------
    // Create Shader Program
    // ------------------------------------------------

    unsigned int shaderProgram =
        createShaderProgram(
            "vShader.glsl",
            "fShader.glsl"
        );


    // ------------------------------------------------
    // Vertex Data
    // ------------------------------------------------

    float vertices[] =
    {
        // Position                 // Texture Coordinate

         0.5f,  0.5f, 0.0f,          1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,          1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,          0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,          0.0f, 1.0f
    };


    // ------------------------------------------------
    // Indices
    // ------------------------------------------------

    unsigned int indices[] =
    {
        0, 1, 3,
        1, 2, 3
    };


    // ------------------------------------------------
    // VAO / VBO / EBO
    // ------------------------------------------------

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    glGenVertexArrays(
        1,
        &VAO
    );

    glGenBuffers(
        1,
        &VBO
    );

    glGenBuffers(
        1,
        &EBO
    );


    // ------------------------------------------------
    // Bind VAO
    // ------------------------------------------------

    glBindVertexArray(VAO);


    // ------------------------------------------------
    // VBO
    // ------------------------------------------------

    glBindBuffer(
        GL_ARRAY_BUFFER,
        VBO
    );

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );


    // ------------------------------------------------
    // EBO
    // ------------------------------------------------

    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        EBO
    );

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );


    // ------------------------------------------------
    // Position Attribute
    // ------------------------------------------------

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);


    // ------------------------------------------------
    // Texture Coordinate Attribute
    // ------------------------------------------------

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(3 * sizeof(float))
    );

    glEnableVertexAttribArray(1);


    // ------------------------------------------------
    // Load Texture
    // ------------------------------------------------

    unsigned int texture;

    glGenTextures(
        1,
        &texture
    );

    glBindTexture(
        GL_TEXTURE_2D,
        texture
    );


    // Texture wrapping
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_S,
        GL_REPEAT
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_WRAP_T,
        GL_REPEAT
    );


    // Texture filtering
    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MIN_FILTER,
        GL_LINEAR
    );

    glTexParameteri(
        GL_TEXTURE_2D,
        GL_TEXTURE_MAG_FILTER,
        GL_LINEAR
    );


    // ------------------------------------------------
    // Load Image
    // ------------------------------------------------

    int width;
    int height;
    int nrChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data =
        stbi_load(
            "Texturelabs_Atmosphere_128S.jpg",
            &width,
            &height,
            &nrChannels,
            0
        );


    if (data)
    {
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            width,
            height,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            data
        );

        glGenerateMipmap(
            GL_TEXTURE_2D
        );

        std::cout
            << "Texture loaded successfully!"
            << std::endl;
    }
    else
    {
        std::cout
            << "ERROR: Failed to load texture!"
            << std::endl;
    }


    stbi_image_free(data);


    // ------------------------------------------------
    // Render Loop
    // ------------------------------------------------

    while (!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);


        // Clear screen
        glClearColor(
            0.2f,
            0.3f,
            0.3f,
            1.0f
        );

        glClear(
            GL_COLOR_BUFFER_BIT
        );


        // ------------------------------------------------
        // Use Shader
        // ------------------------------------------------

        glUseProgram(shaderProgram);


        // ------------------------------------------------
        // Bind Texture
        // ------------------------------------------------

        glActiveTexture(
            GL_TEXTURE0
        );

        glBindTexture(
            GL_TEXTURE_2D,
            texture
        );


        // ------------------------------------------------
        // Transformation
        // ------------------------------------------------

        glm::mat4 transform =
            glm::mat4(1.0f);


        // Move object
        transform =
            glm::translate(
                transform,
                glm::vec3(
                    0.5f,
                    -0.5f,
                    0.0f
                )
            );


        // Rotate object
        transform =
            glm::rotate(
                transform,
                (float)glfwGetTime(),
                glm::vec3(
                    0.0f,
                    0.0f,
                    1.0f
                )
            );


        // ------------------------------------------------
        // Send Transform to Shader
        // ------------------------------------------------

        int transformLoc =
            glGetUniformLocation(
                shaderProgram,
                "transform"
            );

        glUniformMatrix4fv(
            transformLoc,
            1,
            GL_FALSE,
            glm::value_ptr(transform)
        );


        // ------------------------------------------------
        // Texture Sampler
        // ------------------------------------------------

        int textureLoc =
            glGetUniformLocation(
                shaderProgram,
                "texture1"
            );

        glUniform1i(
            textureLoc,
            0
        );


        // ------------------------------------------------
        // Draw
        // ------------------------------------------------

        glBindVertexArray(VAO);

        glDrawElements(
            GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            0
        );


        // ------------------------------------------------
        // Display
        // ------------------------------------------------

        glfwSwapBuffers(window);

        glfwPollEvents();
    }


    // ------------------------------------------------
    // Delete Resources
    // ------------------------------------------------

    glDeleteVertexArrays(
        1,
        &VAO
    );

    glDeleteBuffers(
        1,
        &VBO
    );

    glDeleteBuffers(
        1,
        &EBO
    );

    glDeleteTextures(
        1,
        &texture
    );

    glDeleteProgram(
        shaderProgram
    );


    // ------------------------------------------------
    // Terminate GLFW
    // ------------------------------------------------

    glfwTerminate();

    return 0;
}


// ----------------------------------------------------
// Process Input
// ----------------------------------------------------

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(
        window,
        GLFW_KEY_ESCAPE
    ) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(
            window,
            true
        );
    }
}


// ----------------------------------------------------
// Framebuffer Resize
// ----------------------------------------------------

void framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height)
{
    glViewport(
        0,
        0,
        width,
        height
    );
}