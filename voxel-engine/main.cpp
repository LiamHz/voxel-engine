#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "voxel.h"

const GLint WIDTH = 1920, HEIGHT = 1080;

// Functions
int init();
int processInput(GLFWwindow *window);
int createTexture(unsigned int textures[8], std::string texPath, int i);
int set_vertex_attribs(Shader shader, long vboOffset, int nAttrib);
int create_scene(Shader shader, unsigned int textures[8], unsigned int VAO, unsigned int VBO);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f,  3.0f));
bool firstMouse = true;
float lastX = WIDTH / 2;
float lastY = HEIGHT / 2;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame;

// Misc globals
GLFWwindow *window;
unsigned int texture1;

int main() {
    int nVerts = 36;
    
    // Initialize GLFW and GLAD
    if (init() != 0)
        return -1;
    
    Shader shader("shader.vs", "shader.fs");
    
    // Create voxel chunks
    unsigned int VAO, VBO, textures[8];
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    if (create_scene(shader, textures, VAO, VBO) != 0)
        return -1;
    
    while (!glfwWindowShouldClose(window)) {
        // Per-frame time logic
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);

        // Set view matrix
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("view", view);
        
        // Set world coordinates of cube
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        
        // Render objects
        glBindVertexArray(VAO);
        
        // Bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        shader.setInt("texture1", 0);
        shader.use();
        
        glDrawArrays(GL_TRIANGLES, 0, 2*nVerts);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        shader.setInt("texture1", 1);
        shader.use();
        
        glDrawArrays(GL_TRIANGLES, 2*nVerts, 9*nVerts);
        
        glfwPollEvents();
        
        // Use double buffer
        // Only swap old frame with new when it is completed
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    
    return 0;
}

int create_scene(Shader shader, unsigned int textures[8], unsigned int VAO, unsigned int VBO) {
    long vboOffset = 0;
    
    // Allocate VBO memory
    int nCubes = 11;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, nCubes * sizeof(float) * 180, 0, GL_STATIC_DRAW);
    
    glBindVertexArray(VAO);
    
    createTexture(textures, "container.jpg", 0);
    createTexture(textures, "stone_iron.png", 1);
    
    set_vertex_attribs(shader, vboOffset, 0);
    draw_chunk(-2, 0, -3, 1, 2, 1, VBO, vboOffset);
    
    set_vertex_attribs(shader, vboOffset, 2);
    draw_chunk( 0, 1, -1, 3, 3, 1, VBO, vboOffset);
    
    return 0;
}

int set_vertex_attribs(Shader shader, long vboOffset, int nAttrib) {
    // Configure vertex position and texture attributes
    glVertexAttribPointer(nAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(vboOffset));
    glEnableVertexAttribArray(nAttrib);
    glVertexAttribPointer(nAttrib+1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(vboOffset + 3 * sizeof(float)));
    glEnableVertexAttribArray(nAttrib+1);
    
    return 0;
}

int createTexture(unsigned int textures[8], std::string texPath, int i) {
    glGenTextures(1, &textures[i]);
    glBindTexture(GL_TEXTURE_2D, textures[i]);

    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load and generate texture
    int texWidth, texHeight, nrChannels;
    unsigned char *data = stbi_load(texPath.c_str(), &texWidth, &texHeight, &nrChannels, 0);
    if (data) {
        if (texPath.find(".jpg") != std::string::npos) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if (texPath.find(".png") != std::string::npos) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        } else {
            std::cout << "Error generating texture with glTexImage2D";
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture";
        return -1;
    }

    stbi_image_free(data);

//    shader.setInt("texture"+std::to_string(i), 0);
    
    return 0;
}

// Initialize GLFW and GLAD
int init() {
    glfwInit();
    
    // Set OpenGL window to version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // macOS compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Test", nullptr, nullptr);
    
    // Account for macOS retina resolution
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, screenWidth, screenHeight);
    
    // Enable wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Enable z-buffer
    glEnable(GL_DEPTH_TEST);
    
    // Enable mouse input
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    return 0;
}

int processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    
    return 0;
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    // Prevent camera jumping when mouse first enters screen
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    // yoffset is reversed since y-coords go from bottom to top
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
