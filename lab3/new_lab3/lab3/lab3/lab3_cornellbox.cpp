#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // This header includes glm::value_ptr

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <render/shader.h>

#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

static GLFWwindow *window;
static int windowWidth = 1024;
static int windowHeight = 768;

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
static void cursor_callback(GLFWwindow *window, double xpos, double ypos);

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
//static glm::vec3 lightIntensity = 5.0f * (8.0f * wave500 + 15.6f * wave600 + 18.4f * wave700);
//static glm::vec3 lightPosition(-275.0f, 500.0f, -275.0f);

// Lighting control
static glm::vec3 lightIntensity = glm::vec3(1.0f, 1.0f, 1.0f) * 2.0f; // White light with high intensity
static glm::vec3 lightPosition(-275.0f, 500.0f, -275.0f);
static glm::vec3 ambientLight = glm::vec3(0.5f, 0.5f, 0.5); // Further increase ambient light intensity

// Shadow mapping
static glm::vec3 lightUp(0, 0, 1);
static int shadowMapWidth = 0;
static int shadowMapHeight = 0;

// TODO: set these parameters
static float depthFoV = 0.f;
static float depthNear = 0.f;
static float depthFar = 0.f;

// Helper flag and function to save depth maps for debugging
static bool saveDepth = false;

//Declare the FBO and Depth Texture
GLuint fbo;
GLuint depthTexture;
GLuint depthShaderProgram;

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
    for (int i = 0; i < width * height; ++i) img[3*i] = img[3*i+1] = img[3*i+2] = depth[i] * 255;

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
            // Floor (yellowish color)
            //  glm::vec3 backWallColor(0.36f, 0.25f, 0.20f);
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,

            // Ceiling (yellowish color)
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,

            // Left wall (red)
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            // Right wall (green)
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            // Back wall (yellowish color)
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
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

    // Cube data
    GLfloat cube_vertex_buffer_data[60] = {
            // Define the vertices for a cube
            130.0f, 165.0f,  65.0f,
            82.0f, 165.0f, 225.0f,
            240.0f, 165.0f, 272.0f,
            290.0f, 165.0f, 114.0f,

            290.0f, 0.0f, 114.0f,
            290.0f, 165.0f, 114.0f,
            240.0f, 165.0f, 272.0f,
            240.0f, 0.0f, 272.0f,

            130.0f, 0.0f,  65.0f,
            130.0f, 165.0f,  65.0f,
            290.0f, 165.0f, 114.0f,
            290.0f, 0.0f, 114.0f,

            82.0f, 0.0f, 225.0f,
            82.0f, 165.0f, 225.0f,
            130.0f, 165.0f,  65.0f,
            130.0f, 0.0f,  65.0f,

            240.0f, 0.0f, 272.0f,
            240.0f, 165.0f, 272.0f,
            82.0f, 165.0f, 225.0f,
            82.0f, 0.0f, 225.0f
    };

    GLfloat cube_color_buffer_data[60] = {
            // Colors for each vertex
            // Colors for each vertex (all white)
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
    };
    GLfloat cube_normal_buffer_data[24] = {
            0.0f, 0.0f, -1.0f, // Front
            0.0f, 0.0f, 1.0f,  // Back
            -1.0f, 0.0f, 0.0f, // Left
            1.0f, 0.0f, 0.0f,  // Right
            0.0f, 1.0f, 0.0f,  // Top
            0.0f, -1.0f, 0.0f  // Bottom
    };


    GLuint cube_index_buffer_data[36] = {
            // Front face
            0, 1, 2, 2, 3, 0,
            // Back face
            4, 5, 6, 6, 7, 4,
            // Left face
            8, 9, 10, 10, 11, 8,
            // Right face
            12, 13, 14, 14, 15, 12,
            // Top face
            16, 17, 18, 18, 19, 16,
            // Bottom face
            20, 21, 22, 22, 23, 20

    };

    // Cuboid data
    GLfloat cuboid_vertex_buffer_data[60] = {
            // Define the vertices for a cuboid
            423.0, 330.0, 247.0,
            265.0, 330.0, 296.0,
            314.0, 330.0, 456.0,
            472.0, 330.0, 406.0,

            423.0,   0.0, 247.0,
            423.0, 330.0, 247.0,
            472.0, 330.0, 406.0,
            472.0,   0.0, 406.0,

            472.0,   0.0, 406.0,
            472.0, 330.0, 406.0,
            314.0, 330.0, 456.0,
            314.0,   0.0, 456.0,

            314.0,   0.0, 456.0,
            314.0, 330.0, 456.0,
            265.0, 330.0, 296.0,
            265.0,   0.0, 296.0,

            265.0,   0.0, 296.0,
            265.0, 330.0, 296.0,
            423.0, 330.0, 247.0,
            423.0,   0.0, 247.0,


    };

    GLfloat cuboid_color_buffer_data[60] = {
            // Colors for each vertex
            // Colors for each vertex (all white)
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
            0.36f, 0.25f, 0.20f,
    };
    GLfloat cuboid_normal_buffer_data[24] = {
            0.0f, 0.0f, -1.0f, // Front
            0.0f, 0.0f, 1.0f,  // Back
            -1.0f, 0.0f, 0.0f, // Left
            1.0f, 0.0f, 0.0f,  // Right
            0.0f, 1.0f, 0.0f,  // Top
            0.0f, -1.0f, 0.0f  // Bottom
    };


    GLuint cuboid_index_buffer_data[36] = {
            // Front face
            0, 1, 2, 2, 3, 0,
            // Back face
            4, 5, 6, 6, 7, 4,
            // Left face
            8, 9, 10, 10, 11, 8,
            // Right face
            12, 13, 14, 14, 15, 12,
            // Top face
            16, 17, 18, 18, 19, 16,
            // Bottom face
            20, 21, 22, 22, 23, 20

    };

    // OpenGL buffers for the cube
    GLuint cubeVertexArrayID;
    GLuint cubeVertexBufferID;
    GLuint cubeColorBufferID;
    GLuint cubeIndexBufferID;

    // OpenGL buffers for the cuboid
    GLuint cuboidVertexArrayID;
    GLuint cuboidVertexBufferID;
    GLuint cuboidColorBufferID;
    GLuint cuboidIndexBufferID;


    // OpenGL buffers
    GLuint vertexArrayID;
    GLuint vertexBufferID;
    GLuint indexBufferID;
    GLuint colorBufferID;
    GLuint normalBufferID;

    // Shader variable IDs
    GLuint mvpMatrixID;
    GLuint lightPositionID;
    GLuint lightIntensityID;
    GLuint ambientLightID;
    GLuint programID;


    void initialize() {

        // Create a vertex array object
        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);

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

        // Create and compile our GLSL program from the shaders
        programID = LoadShadersFromFile("../lab3/box.vert", "../lab3/box.frag");
        if (programID == 0) {
            std::cerr << "Failed to load shaders." << std::endl;
        }

        // Create and compile the depth shader program for shadow mapping
        depthShaderProgram = LoadShadersFromFile("../lab3/depth.vert", "../lab3/depth.frag");
        if (depthShaderProgram == 0) {
            std::cerr << "Failed to load depth shaders." << std::endl;
        }

        // Set up the light's view and projection matrices
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
        glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        // Store the lightSpaceMatrix in a uniform location for use in shaders
        GLuint lightSpaceMatrixID = glGetUniformLocation(programID, "lightSpaceMatrix");
        glUniformMatrix4fv(lightSpaceMatrixID, 1, GL_FALSE, &lightSpaceMatrix[0][0]);

        // Get a handle for our "MVP" uniform
        mvpMatrixID = glGetUniformLocation(programID, "MVP");
        lightPositionID = glGetUniformLocation(programID, "lightPosition");
        lightIntensityID = glGetUniformLocation(programID, "lightIntensity");
        ambientLightID = glGetUniformLocation(programID, "ambientLight"); // Get ambient light uniform location

        // Create a vertex array object for the cube
        glGenVertexArrays(1, &cubeVertexArrayID);
        glBindVertexArray(cubeVertexArrayID);

        // Create a vertex buffer object for the cube's vertices
        glGenBuffers(1, &cubeVertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertex_buffer_data), cube_vertex_buffer_data, GL_STATIC_DRAW);

        // Create a color buffer object for the cube's colors
        glGenBuffers(1, &cubeColorBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, cubeColorBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color_buffer_data), cube_color_buffer_data, GL_STATIC_DRAW);

        // Create an index buffer object for the cube's indices
        glGenBuffers(1, &cubeIndexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_index_buffer_data), cube_index_buffer_data, GL_STATIC_DRAW);

        // Create a vertex array object for the cuboid
        glGenVertexArrays(1, &cuboidVertexArrayID);
        glBindVertexArray(cuboidVertexArrayID);

        // Create a vertex buffer object for the cuboid's vertices
        glGenBuffers(1, &cuboidVertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, cuboidVertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cuboid_vertex_buffer_data), cuboid_vertex_buffer_data, GL_STATIC_DRAW);

        // Create a color buffer object for the cuboid's colors
        glGenBuffers(1, &cuboidColorBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, cuboidColorBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cuboid_color_buffer_data), cuboid_color_buffer_data, GL_STATIC_DRAW);

        // Create an index buffer object for the cuboid's indices
        glGenBuffers(1, &cuboidIndexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cuboidIndexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cuboid_index_buffer_data), cuboid_index_buffer_data, GL_STATIC_DRAW);

        // Create and configure the FBO
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        // Create the depth texture
        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Attach the depth texture to the FBO
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
        glDrawBuffer(GL_NONE); // No color buffer is drawn to
        glReadBuffer(GL_NONE);

        // Check FBO completeness
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Error: Framebuffer is not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind the FBO

    }



    void render(glm::mat4 cameraMatrix) {

        glUseProgram(programID);

        // Set light data
        glUniform3fv(lightPositionID, 1, &lightPosition[0]);
        glUniform3fv(lightIntensityID, 1, &lightIntensity[0]);
        glUniform3fv(ambientLightID, 1, &ambientLight[0]); // Pass ambient light

        // Render the Cornell Box
        glBindVertexArray(vertexArrayID);

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

        // Set model-view-projection matrix for the Cornell Box
        glm::mat4 modelMatrixBox = glm::mat4(1.0f); // Identity matrix for the box
        glm::mat4 mvpBox = cameraMatrix * modelMatrixBox;
        glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvpBox[0][0]);

        // Draw the Cornell Box
        glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        // Render the Cube
        glBindVertexArray(cubeVertexArrayID);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, cubeColorBufferID);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeIndexBufferID);

        // Set model-view-projection matrix for the cube
        glm::mat4 modelMatrixCube = glm::translate(glm::mat4(1.0f),
                                                   glm::vec3(-325.0f, 50.0f, -275.0f)); // Adjust position as needed
        glm::mat4 mvpCube = cameraMatrix * modelMatrixCube;
        glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvpCube[0][0]);

        // Draw the cube
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        // Render the Cuboid
        glBindVertexArray(cuboidVertexArrayID);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, cuboidVertexBufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, cuboidColorBufferID);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cuboidIndexBufferID);

        // Set model-view-projection matrix for the cube
        float scaleFactor = 0.80f; // Scale the cube down to half its size
        // Set model-view-projection matrix for the cuboid
        glm::mat4 modelMatrixCuboid = glm::translate(glm::mat4(1.0f),
                                                     glm::vec3(-610.0f, 90.0f, -275.0f)); // Adjust position as needed

        modelMatrixCuboid = glm::scale(modelMatrixCuboid, glm::vec3(scaleFactor, scaleFactor, scaleFactor)); // Apply scaling
        glm::mat4 mvpCuboid = cameraMatrix * modelMatrixCuboid;
        glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvpCuboid[0][0]);

        // Draw the cuboid
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

    };

    void cleanup() {
        glDeleteBuffers(1, &vertexBufferID);
        glDeleteBuffers(1, &colorBufferID);
        glDeleteBuffers(1, &indexBufferID);
        glDeleteBuffers(1, &normalBufferID);
        glDeleteVertexArrays(1, &vertexArrayID);
        glDeleteProgram(programID);
        glDeleteBuffers(1, &cubeVertexBufferID);
        glDeleteBuffers(1, &cubeColorBufferID);
        glDeleteBuffers(1, &cubeIndexBufferID);
        glDeleteVertexArrays(1, &cubeVertexArrayID);
        glDeleteBuffers(1, &cuboidVertexBufferID);
        glDeleteBuffers(1, &cuboidColorBufferID);
        glDeleteBuffers(1, &cuboidIndexBufferID);
        glDeleteVertexArrays(1, &cuboidVertexArrayID);
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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 viewMatrix = glm::lookAt(eye_center, lookat, up);
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(FoV), (float)windowWidth / windowHeight, zNear, zFar);
        glm::mat4 cameraMatrix = projectionMatrix * viewMatrix;

        b.render(cameraMatrix);

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

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
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

static void cursor_callback(GLFWwindow *window, double xpos, double ypos) {
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

    std::cout << lightPosition.x << " " << lightPosition.y << " " << lightPosition.z << std::endl;
}

