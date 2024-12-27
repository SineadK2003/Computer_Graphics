#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <render/shader.h>

#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

static GLFWwindow* window;
static int windowWidth = 1024;
static int windowHeight = 768;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
static void cursor_callback(GLFWwindow* window, double xpos, double ypos);

// OpenGL camera view parameters
static glm::vec3 eye_center(-278.0f, 273.0f, 800.0f);
static glm::vec3 lookat(-278.0f, 273.0f, 0.0f);
static glm::vec3 up(0.0f, 1.0f, 0.0f);
static float FoV = 45.0f;
static float zNear = 600.0f;
static float zFar = 1500.0f;

// Lighting control
const glm::vec3 wave500(0.0f, 255.0f, 146.0f);
const glm::vec3 wave600(255.0f, 190.0f, 0.0f);
const glm::vec3 wave700(205.0f, 0.0f, 0.0f);
static glm::vec3 lightIntensity = 5.0f * (8.0f * wave500 + 15.6f * wave600 + 18.4f * wave700);
static glm::vec3 lightPosition(-275.0f, 500.0f, -275.0f);

// Shadow mapping
static glm::vec3 lightUp(0.0f, -1.0f, 0.0f); //Light facing down
GLuint shadowFBO;
glm::vec3 boxCenter(-275.0f, 274.4f, -279.6f);
GLuint depthTexture;
GLuint depthProgramID;
GLuint lightSpaceMatrixLocation;
glm::mat4 lightSpaceMatrix;
static int shadowMapWidth = 1024;
static int shadowMapHeight = 1024;

// TODO: set these parameters
static float depthFoV = 90.0f;
static float depthNear = 90.0f;
static float depthFar = 1000.0f;

// Helper flag and function to save depth maps for debugging
static bool saveDepth = false;

// This function retrieves and stores the depth map of the default frame buffer
// or a particular frame buffer (indicated by FBO ID) to a PNG image.
static void saveDepthTexture(GLuint fbo, std::string filename) {
    int width = shadowMapWidth;
    int height = shadowMapHeight;
    if (shadowMapWidth == 0 || shadowMapHeight == 0) {
        width = windowWidth;
        height = windowHeight;
    }
    int channels = 3;

    std::vector<float> depth(width * height);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glReadBuffer(GL_DEPTH_COMPONENT);
    glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depth.data());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    std::vector<unsigned char> img(width * height * 3);
    for (int i = 0; i < width * height; ++i) img[3 * i] = img[3 * i + 1] = img[3 * i + 2] = depth[i] * 255;

    stbi_write_png(filename.c_str(), width, height, channels, img.data(), width * channels);
}

struct CornellBox {

    // Refer to original Cornell Box data
    // from https://www.graphics.cornell.edu/online/box/data.html

    GLfloat vertex_buffer_data[60] = {
            // Floor
            -552.8, 0.0, 0.0,
            0.0, 0.0,   0.0,
            0.0, 0.0, -559.2,
            -549.6, 0.0, -559.2,

            // Ceiling
            -556.0, 548.8, 0.0,
            -556.0, 548.8, -559.2,
            0.0, 548.8, -559.2,
            0.0, 548.8,   0.0,

            // Left wall
            -552.8,   0.0,   0.0,
            -549.6,   0.0, -559.2,
            -556.0, 548.8, -559.2,
            -556.0, 548.8,   0.0,

            // Right wall
            0.0,   0.0, -559.2,
            0.0,   0.0,   0.0,
            0.0, 548.8,   0.0,
            0.0, 548.8, -559.2,

            // Back wall
            -549.6,   0.0, -559.2,
            0.0,   0.0, -559.2,
            0.0, 548.8, -559.2,
            -556.0, 548.8, -559.2
    };

    // TODO: set vertex normals properly
    GLfloat normal_buffer_data[60] = {
            // Floor
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,

            // Ceiling
            0.0, -1.0, 0.0,
            0.0, -1.0, 0.0,
            0.0, -1.0, 0.0,
            0.0, -1.0, 0.0,

            // Left wall
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,

            // Right wall
            -1.0, 0.0, 0.0,
            -1.0, 0.0, 0.0,
            -1.0, 0.0, 0.0,
            -1.0, 0.0, 0.0,

            // Back wall
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0
    };

    GLfloat color_buffer_data[60] = {
            // Floor
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,

            // Ceiling
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,

            // Left wall
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            // Right wall
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            // Back wall
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f
    };

    GLuint index_buffer_data[30] = {
            0, 1, 2,
            0, 2, 3,

            4, 5, 6,
            4, 6, 7,

            8, 9, 10,
            8, 10, 11,

            12, 13, 14,
            12, 14, 15,

            16, 17, 18,
            16, 18, 19,
    };

    // Vertex data for the short block
    GLfloat short_block_vertex_buffer_data[60] = {
            // Top
            -130.0f, 165.0f, -65.0f,
            -82.0f, 165.0f, -225.0f,
            -240.0f, 165.0f, -272.0f,
            -290.0f, 165.0f, -114.0f,


            // Left
            -290.0f, 0.0f, -114.0f,
            -290.0f, 165.0f, -114.0f,
            -240.0f, 165.0f, -272.0f,
            -240.0f, 0.0f, -272.0f,


            // Front
            -130.0f, 0.0f, -65.0f,
            -130.0f, 165.0f, -65.0f,
            -290.0f, 165.0f, -114.0f,
            -290.0f, 0.0f, -114.0f,


            // Right
            -82.0f, 0.0f, -225.0f,
            -82.0f, 165.0f, -225.0f,
            -130.0f, 165.0f, -65.0f,
            -130.0f, 0.0f, -65.0f,

            // Back
            -240.0f, 0.0f, -272.0f,
            -240.0f, 165.0f, -272.0f,
            -82.0f, 165.0f, -225.0f,
            -82.0f, 0.0f, -225.0f,
    };

    // Normal data (unmodified from original structure)
    GLfloat short_block_normal_buffer_data[60] = {
            // Top face (Normal points up)
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            // Left face
            -0.9534f, 0.0f, -0.3017f,
            -0.9534f, 0.0f, -0.3017f,
            -0.9534f, 0.0f, -0.3017f,
            -0.9534f, 0.0f, -0.3017f,

            // Front face
            -0.2928f, 0.0f, 0.9562f,
            -0.2928f, 0.0f, 0.9562f,
            -0.2928f, 0.0f, 0.9562f,
            -0.2928f, 0.0f, 0.9562f,

            // Right face
            0.9578f, 0.0f, 0.2873f,
            0.9578f, 0.0f, 0.2873f,
            0.9578f, 0.0f, 0.2873f,
            0.9578f, 0.0f, 0.2873f,

            // Back face
            0.2851, 0.0, -0.9585f,
            0.2851, 0.0, -0.9585f,
            0.2851, 0.0, -0.9585f,
            0.2851, 0.0, -0.9585f,
    };

    // Color buffer data (white for all faces)
    GLfloat short_block_color_buffer_data[60] = {
            // Floor
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,

            // Ceiling
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,

            // Left wall
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,

            // Right wall
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,

            // Back wall
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
    };

    // Index data (defines the triangles)
    GLuint short_block_index_buffer_data[30] = {
            0, 1, 2,
            0, 2, 3,

            4, 5, 6,
            4, 6, 7,

            8, 9, 10,
            8, 10, 11,

            12, 13, 14,
            12, 14, 15,

            16, 17, 18,
            16, 18, 19,
    };

    // Vertex data for the tall block
    GLfloat tall_block_vertex_buffer_data[60] = {
            // Top wall
            -423.0f, 330.0f, -247.0f,
            -265.0f, 330.0f, -296.0f,
            -314.0f, 330.0f, -456.0f,
            -472.0f, 330.0f, -406.0f,

            // Left wall
            -423.0f, 0.0f, -247.0f,
            -423.0f, 330.0f, -247.0f,
            -472.0f, 330.0f, -406.0f,
            -472.0f, 0.0f, -406.0f,

            // Right wall
            -472.0f, 0.0f, -406.0f,
            -472.0f, 330.0f, -406.0f,
            -314.0f, 330.0f, -456.0f,
            -314.0f, 0.0f, -456.0f,

            // Front wall
            -314.0f, 0.0f, -456.0f,
            -314.0f, 330.0f, -456.0f,
            -265.0f, 330.0f, -296.0f,
            -265.0f, 0.0f, -296.0f,

            // Back wall
            -265.0f, 0.0f, -296.0f,
            -265.0f, 330.0f, -296.0f,
            -423.0f, 330.0f, -247.0f,
            -423.0f, 0.0f, -247.0f
    };

    // Normal data (unmodified from original structure)
    GLfloat tall_block_normal_buffer_data[60] = {
            // Top face (Normal points up)
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            // Left face (Normal points left)
            -0.9556f,0.0f,0.2945f,
            -0.9556f,0.0f,0.2945f,
            -0.9556f,0.0f,0.2945f,
            -0.9556f,0.0f,0.2945f,

            // Front face
            -0.3017f,0.0f,-0.9534f,
            -0.3017f,0.0f,-0.9534f,
            -0.3017f,0.0f,-0.9534f,
            -0.3017f,0.0f,-0.9534f,

            // Right face
            0.9562f,0.0f,-0.2928f,
            0.9562f,0.0f,-0.2928f,
            0.9562f,0.0f,-0.2928f,
            0.9562f,0.0f,-0.2928f,

            // Back face
            0.2962f,0.0f,0.9551f,
            0.2962f,0.0f,0.9551f,
            0.2962f,0.0f,0.9551f,
            0.2962f,0.0f,0.9551f,
    };

    // Color buffer data (white for all faces)
    GLfloat tall_block_color_buffer_data[60] = {
            // Floor
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,

            // Ceiling
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,

            // Left wall
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,

            // Right wall
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,

            // Back wall
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
    };

    // Index data (defines the triangles)
    GLuint tall_block_index_buffer_data[30] = {
            0, 1, 2,
            0, 2, 3,

            4, 5, 6,
            4, 6, 7,

            8, 9, 10,
            8, 10, 11,

            12, 13, 14,
            12, 14, 15,

            16, 17, 18,
            16, 18, 19,
    };

    // OpenGL buffers
    GLuint vertexArrayID;
    GLuint vertexBufferID;
    GLuint indexBufferID;
    GLuint colorBufferID;
    GLuint normalBufferID;

    // OpenGL buffers for the short box
    GLuint shortBlockVertexBufferID;
    GLuint shortBlockNormalBufferID;
    GLuint shortBlockColorBufferID;
    GLuint shortBlockIndexBufferID;

    // OpenGL buffers for the tall box
    GLuint tallBlockVertexBufferID;
    GLuint tallBlockNormalBufferID;
    GLuint tallBlockColorBufferID;
    GLuint tallBlockIndexBufferID;

    // Shader variable IDs
    GLuint mvpMatrixID;
    GLuint lightPositionID;
    GLuint lightIntensityID;
    GLuint programID;
    GLuint lightSpaceMatrixID;

    void initialize() {
        // Create a vertex array object
        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);

        //Create FBO
        glGenFramebuffers(1, &shadowFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        // Create a vertex buffer object to store the vertex data
        glGenBuffers(1, &vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

        // Create a vertex buffer object to store the color data
        glGenBuffers(1, &colorBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);

        // Create a vertex buffer object to store the vertex normals
        glGenBuffers(1, &normalBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normal_buffer_data), normal_buffer_data, GL_STATIC_DRAW);

        // Create an index buffer object to store the index data that defines triangle faces
        glGenBuffers(1, &indexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_STATIC_DRAW);

        // Create buffers for short box
        glGenBuffers(1, &shortBlockVertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, shortBlockVertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(short_block_vertex_buffer_data), short_block_vertex_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &shortBlockNormalBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, shortBlockNormalBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(short_block_normal_buffer_data), short_block_normal_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &shortBlockColorBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, shortBlockColorBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(short_block_color_buffer_data), short_block_color_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &shortBlockIndexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shortBlockIndexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short_block_index_buffer_data), short_block_index_buffer_data, GL_STATIC_DRAW);

        // Create buffers for tall box
        glGenBuffers(1, &tallBlockVertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, tallBlockVertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tall_block_vertex_buffer_data), tall_block_vertex_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &tallBlockNormalBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, tallBlockNormalBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tall_block_normal_buffer_data), tall_block_normal_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &tallBlockColorBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, tallBlockColorBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tall_block_color_buffer_data), tall_block_color_buffer_data, GL_STATIC_DRAW);

        glGenBuffers(1, &tallBlockIndexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tallBlockIndexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tall_block_index_buffer_data), tall_block_index_buffer_data, GL_STATIC_DRAW);


        // Create and compile our GLSL program from the shaders
        // Create and compile our GLSL program from the shaders
        programID = LoadShadersFromFile("C:\\Computer_Graphics_Git\\Computer_Graphics\\lab3\\new_lab3\\lab3\\lab3\\box.vert", "C:\\Computer_Graphics_Git\\Computer_Graphics\\lab3\\new_lab3\\lab3\\lab3\\box.frag");
        if (programID == 0) {
            std::cerr << "Failed to load shaders." << std::endl;
        }

        // Create and compile the depth shader program for shadow mapping
        depthProgramID = LoadShadersFromFile("C:\\Computer_Graphics_Git\\Computer_Graphics\\lab3\\new_lab3\\lab3\\lab3\\depth.vert", "C:\\Computer_Graphics_Git\\Computer_Graphics\\lab3\\new_lab3\\lab3\\lab3\\depth.frag");
        if (depthProgramID == 0) {
            std::cerr << "Failed to load depth shaders." << std::endl;
        }
        lightSpaceMatrixLocation = glGetUniformLocation(depthProgramID, "lightSpaceMatrix");

        // Get a handle for our "MVP" uniform
        mvpMatrixID = glGetUniformLocation(programID, "MVP");
        lightPositionID = glGetUniformLocation(programID, "lightPosition");
        lightIntensityID = glGetUniformLocation(programID, "lightIntensity");
        lightSpaceMatrixID = glGetUniformLocation(programID, "lightSpaceMatrix");
    }

    void render(glm::mat4 cameraMatrix, bool renderDepth = false) {
        if (renderDepth) {
            glUseProgram(depthProgramID);

            // Main Box
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
            glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, (void*)0);

            // Short Box
            glBindBuffer(GL_ARRAY_BUFFER, shortBlockVertexBufferID);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shortBlockIndexBufferID);
            glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, (void*)0);

            // Tall Box
            glBindBuffer(GL_ARRAY_BUFFER, tallBlockVertexBufferID);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tallBlockIndexBufferID);
            glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, (void*)0);

            glDisableVertexAttribArray(0);
        }
        else {
            glUseProgram(programID);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depthTexture);
            glUniform1i(glGetUniformLocation(programID, "shadowMap"), 1);

            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

            // Set model-view-projection matrix
            glm::mat4 mvp = cameraMatrix;
            glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);

            // Set light data
            glUniform3fv(lightPositionID, 1, &lightPosition[0]);
            glUniform3fv(lightIntensityID, 1, &lightIntensity[0]);

            // Draw the box
            glDrawElements(
                    GL_TRIANGLES,      // mode
                    30,    			   // number of indices
                    GL_UNSIGNED_INT,   // type
                    (void*)0           // element array buffer offset
            );

            // Render the short block
            glBindBuffer(GL_ARRAY_BUFFER, shortBlockVertexBufferID);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, shortBlockColorBufferID);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, shortBlockNormalBufferID);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shortBlockIndexBufferID);

            glDisable(GL_CULL_FACE);

            // Draw short block
            glDrawElements(
                    GL_TRIANGLES,      // mode
                    30,    			   // number of indices
                    GL_UNSIGNED_INT,   // type
                    (void*)0           // element array buffer offset
            );

            // Render the tall box
            glBindBuffer(GL_ARRAY_BUFFER, tallBlockVertexBufferID);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, tallBlockColorBufferID);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ARRAY_BUFFER, tallBlockNormalBufferID);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tallBlockIndexBufferID);

            // Set the MVP matrix if needed for the tall box
            glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &cameraMatrix[0][0]);

            // Draw tall box
            glDrawElements(
                    GL_TRIANGLES,      // mode
                    30,    			   // number of indices
                    GL_UNSIGNED_INT,   // type
                    (void*)0           // element array buffer offset
            );

            glEnable(GL_CULL_FACE);

            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
        }
    }

    void cleanup() {
        glDeleteBuffers(1, &vertexBufferID);
        glDeleteBuffers(1, &colorBufferID);
        glDeleteBuffers(1, &indexBufferID);
        glDeleteBuffers(1, &normalBufferID);
        glDeleteVertexArrays(1, &vertexArrayID);
        glDeleteProgram(programID);
    }
};


int main(void)
{
    // Initialise GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For MacOS
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(windowWidth, windowHeight, "Lab 3", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to open a GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetKeyCallback(window, key_callback);

    glfwSetCursorPosCallback(window, cursor_callback);

    // Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        std::cerr << "Failed to initialize OpenGL context." << std::endl;
        return -1;
    }

    // Prepare shadow map size for shadow mapping. Usually this is the size of the window itself, but on some platforms like Mac this can be 2x the size of the window. Use glfwGetFramebufferSize to get the shadow map size properly.
    glfwGetFramebufferSize(window, &shadowMapWidth, &shadowMapHeight);

    // Background
    glClearColor(0.2f, 0.2f, 0.25f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Create the classical Cornell Box
    CornellBox b;
    b.initialize();

    // Camera setup
    glm::mat4 viewMatrix, projectionMatrix;
    projectionMatrix = glm::perspective(glm::radians(FoV), (float)windowWidth / windowHeight, zNear, zFar);

    do
    {
        glViewport(0, 0, shadowMapWidth, shadowMapHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Depth map pass
        glm::mat4 lightProjection = glm::perspective(glm::radians(depthFoV), (float)shadowMapWidth / shadowMapHeight, depthNear, depthFar);
        glm::mat4 lightView = glm::lookAt(lightPosition, boxCenter, lightUp);
        lightSpaceMatrix = lightProjection * lightView;

        // Render shadow map to shadow framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glUseProgram(depthProgramID);
        glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
        b.render(lightSpaceMatrix, true); // Render scene from light's perspective

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //2nd pass, rendering scene normally
        glViewport(0, 0, windowWidth, windowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        viewMatrix = glm::lookAt(eye_center, lookat, up);
        glm::mat4 vp = projectionMatrix * viewMatrix;

        glUseProgram(b.programID);
        glUniformMatrix4fv(b.lightSpaceMatrixID, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
        b.render(vp);

        if (saveDepth) {
            std::string filename = "depth_camera.png";
            saveDepthTexture(0, filename);
            std::cout << "Depth texture saved to " << filename << std::endl;
            saveDepth = false;
        }

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (!glfwWindowShouldClose(window));

    // Clean up
    b.cleanup();

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        eye_center = glm::vec3(-278.0f, 273.0f, 800.0f);
        lightPosition = glm::vec3(-275.0f, 500.0f, -275.0f);

    }

    if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        eye_center.y += 20.0f;
    }

    if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        eye_center.y -= 20.0f;
    }

    if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        eye_center.x -= 20.0f;
    }

    if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        eye_center.x += 20.0f;
    }

    if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        lightPosition.z -= 20.0f;

    }

    if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        lightPosition.z += 20.0f;
    }

    if (key == GLFW_KEY_SPACE && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        saveDepth = true;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    if (xpos < 0 || xpos >= windowWidth || ypos < 0 || ypos > windowHeight)
        return;

    // Normalize to [0, 1]
    float x = xpos / windowWidth;
    float y = ypos / windowHeight;

    // To [-1, 1] and flip y up
    x = x * 2.0f - 1.0f;
    y = 1.0f - y * 2.0f;

    const float scale = 250.0f;
    lightPosition.x = x * scale - 278;
    lightPosition.y = y * scale + 278;

    //std::cout << lightPosition.x << " " << lightPosition.y << " " << lightPosition.z << std::endl;
}
