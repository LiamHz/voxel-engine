#include <iostream>
#include <cmath>
#include <numeric>
#include <functional>

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
#include "scenes.h"

using std::vector;

const GLint WIDTH = 1920, HEIGHT = 1080;

// Structs

// Functions
int init();
int processInput(GLFWwindow *window);
inline int get_self_product(vector<int> x);
int set_vertex_attribs(long vboOffset, int nAttrib);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
int create_scene(Shader shader, unsigned int VAO, unsigned int VBO, vector<vector<int>> &dim, vector<int> &col, std::vector<glm::vec3> &palette);

// Camera
Camera camera(glm::vec3(4.0f, 4.0f, 10.0f));
bool firstMouse = true;
float lastX = WIDTH / 2;
float lastY = HEIGHT / 2;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame;

// Misc globals
GLFWwindow *window;

int main() {
    int vertOffset;
    int vertSize;
    int nVerts = 36;
    
    // Initialize GLFW and GLAD
    if (init() != 0)
        return -1;
    
    Shader objectShader("objectShader.vs", "objectShader.fs");
    Shader lampShader("lampShader.vs", "lampShader.fs");
    
    // Create voxel chunks
    unsigned int VAO, VBO, lightVAO, lightVBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &lightVBO);
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &lightVAO);
    vector<vector<int>> dim;
    vector<int> col;
    std::vector<glm::vec3> palette;
    
    if (create_scene(objectShader, VAO, VBO, dim, col, palette) != 0)
        return -1;
    
    // Set origin world coordinates
    glm::mat4 model = glm::mat4(1.0f);
    objectShader.use();
    objectShader.setMat4("model", model);
    
    // Lighting
    objectShader.setVec3("u_lightColor", 1.0f, 1.0f, 1.0f);
    objectShader.setMat4("model", model);
    
    std::vector<float> lightCube = get_cube(0, 0, 0);
    
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, lightCube.size() * sizeof(float), lightCube.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    while (!glfwWindowShouldClose(window)) {
        vertSize = 0;
        vertOffset = 0;
        
        // Per-frame time logic
        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        processInput(window);
        objectShader.use();
        
        glClearColor(54/255.0f, 81/255.0f, 94/255.0f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Dynamic lighting
        glm::vec3 lightPos(glm::sin(glfwGetTime()) * 15, 10.0f, glm::sin(glfwGetTime()) * 15);
        objectShader.setVec3("u_lightPos", lightPos);

        // Set projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        objectShader.setMat4("projection", projection);

        // Set view matrix
        glm::mat4 view = camera.GetViewMatrix();
        objectShader.setMat4("view", view);
        
        glBindVertexArray(VAO);
        objectShader.setVec3("viewPos", camera.Position);
        // Render each object in VBO with its associated texture
        for (int i = 0; i < dim.size(); i++) {
            vertSize = get_self_product(dim[i]) * nVerts;
            objectShader.setVec3("u_objectColor", palette[col[i]]);
            glDrawArrays(GL_TRIANGLES, vertOffset, vertSize);
            vertOffset += vertSize;
        }
        
        // Draw lamp
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        lampShader.setMat4("model", model);
        // glBindVertexArray(lightVAO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        
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

inline int get_self_product(vector<int> x) {
    return x[0] * x[1] * x[2];
}

int create_scene(Shader shader, unsigned int VAO, unsigned int VBO, vector<vector<int>> &dim, vector<int> &col, std::vector<glm::vec3> &palette) {
    int nCubes = 1024;
    long vboOffset = 0;
    vector<vector<int>> pos;
    chunkVecs cv(pos, dim, col);
    chunkInfo ci(VBO, vboOffset, 0);
    
    // Allocate VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, nCubes * sizeof(float) * 180, 0, GL_STATIC_DRAW);
    
    glBindVertexArray(VAO);
    
    draw_scene("pizza", cv, palette);
    
    for (int i = 0; i < dim.size(); i++)
        draw_chunk(pos[i], dim[i], ci);
    
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
    
    window = glfwCreateWindow(WIDTH, HEIGHT, "Voxel Engine", nullptr, nullptr);
    
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
