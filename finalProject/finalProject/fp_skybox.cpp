#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#include <stb/stb_image.h>

#include <render/shader.h>

#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

static GLFWwindow *window;
static int windowWidth = 1024;
static int windowHeight = 768;
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// OpenGL camera view parameters
//static glm::vec3 eye_center(0.0f, 0.0f, 0.0f);
static glm::vec3 lookat(0.0f, 0.0f, 0.0f);
glm::vec3 eye_center(0.0f, 50.0f, 200.0f); // Fixed camera position
//glm::vec3 lookat(0.0f, 0.0f, 0.0f); // Initial lookAt point

// Global variables for camera control
//glm::vec3 eye_center(0.0f, 50.0f, 200.0f); // Initial camera position
//glm::vec3 lookat(0.0f, 0.0f, 0.0f); // Initial lookAt point
//glm::vec3 up(0.0f, 1.0f, 0.0f); // Up vector

// View control
static float viewAzimuth = 0.f;
static float viewPolar = 0.f;
static float viewDistance = 300.0f;
// Global variables for camera control
//float viewAzimuth = glm::radians(45.0f); // Angle in the XZ-plane
//float viewPolar = glm::radians(30.0f);   // Elevation angle
//float viewDistance = 120.0f;             // Distance from the center


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
static void cursor_callback(GLFWwindow* window, double xpos, double ypos);

// OpenGL camera view parameters
//static glm::vec3 eye_center(-278.0f, 273.0f, 800.0f);
//static glm::vec3 lookat(-278.0f, 273.0f, 0.0f);
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

static GLuint LoadTextureTileBox(const char *texture_file_path) {
    int w, h, channels;
    uint8_t* img = stbi_load(texture_file_path, &w, &h, &channels, 3);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // To tile textures on a box, we set wrapping to repeat
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    if (img) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture " << texture_file_path << std::endl;
    }
    stbi_image_free(img);

    return texture;
}

struct Skybox {
    glm::vec3 position;		// Position of the box
    glm::vec3 scale;		// Size of the box in each axis

    GLfloat vertex_buffer_data[72] = {	// Vertex definition for a canonical box
            // Front face
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,

            // Back face
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,

            // Left face
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            // Right face
            1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,

            // Top face
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            // Bottom face
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,
    };

    GLfloat color_buffer_data[72] = {
            // Front, red
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            // Back, yellow
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,

            // Left, green
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            // Right, cyan
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,

            // Top, blue
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            // Bottom, magenta
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
    };

    GLuint index_buffer_data[36] = {		// 12 triangle faces of a box
            0, 2, 1,
            0, 3, 2,

            4, 6, 5,
            4, 7, 6,

            8, 10, 9,
            8, 11, 10,

            12, 14, 13,
            12, 15, 14,

            16, 18, 17,
            16, 19, 18,

            20, 22, 21,
            20, 23, 22,
    };

    GLfloat uv_buffer_data[48] = {
            // Front (+Z)
            0.5f, 0.666f,
            0.25f, 0.666f,
            0.25f, 0.333f,
            0.5f, 0.333f,

            // Back (-Z)
            1.0f, 0.665f,
            0.75f, 0.665f,
            0.75f, 0.334f,
            1.0f, 0.334f,

            // Left (+X)
            0.75f, 0.665f,
            0.5f, 0.665f,
            0.5f, 0.334f,
            0.75f, 0.334f,

            // Right (-X)
            0.25f, 0.665f,
            0.0f, 0.665f,
            0.0f, 0.334f,
            0.25f, 0.334f,

            // Top (+Y)
            0.499f, 0.333f,
            0.251f, 0.333f,
            0.251f, 0.0f,
            0.499f, 0.0f,

            // Bottom (-Y)
            0.499f, 1.0,
            0.251f, 1.0f,
            0.251f, 0.666f,
            0.499f, 0.666f
    };

    // OpenGL buffers
    GLuint vertexArrayID;
    GLuint vertexBufferID;
    GLuint indexBufferID;
    GLuint colorBufferID;
    GLuint uvBufferID;
    GLuint textureID;

    // Shader variable IDs
    GLuint mvpMatrixID;
    GLuint textureSamplerID;
    GLuint programID;

    void initialize(glm::vec3 position, glm::vec3 scale, const char* texturePath) {
        // Define scale of the building geometry
        this->position = position;
        this->scale = scale;

        // Create a vertex array object
        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);

        // Create a vertex buffer object to store the vertex data
        glGenBuffers(1, &vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1, &uvBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data, GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Create an index buffer object to store the index data that defines triangle faces
        glGenBuffers(1, &indexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_STATIC_DRAW);

        // Create and compile our GLSL program from the shaders
        programID = LoadShadersFromFile("C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\skybox.vert", "C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\skybox.frag");
        if (programID == 0) {
            std::cerr << "Failed to load shaders." << std::endl;
        }

        // Get a handle for our "MVP" uniform
        mvpMatrixID = glGetUniformLocation(programID, "MVP");

        // TODO: Load a texture
        // --------------------
        // --------------------
        textureID = LoadTextureTileBox(texturePath);

        // TODO: Get a handle to texture sampler
        // -------------------------------------
        // -------------------------------------
        textureSamplerID = glGetUniformLocation(programID, "textureSampler");
    }

    void render(glm::mat4 cameraMatrix) {
        glUseProgram(programID);

        glBindVertexArray(vertexArrayID);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

        // Model transform
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, scale.y / 2.0f, 0.0f));
        // Scale the box along each axis
        modelMatrix = glm::scale(modelMatrix, scale);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));

        // Set model-view-projection matrix
        glm::mat4 mvp = cameraMatrix * modelMatrix;
        glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);

        // Enable UV buffer and texture sampler
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Set textureSampler to use texture unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(textureSamplerID, 0);

        // Draw the box
        glDrawElements(
                GL_TRIANGLES,      // mode
                36,    			   // number of indices
                GL_UNSIGNED_INT,   // type
                (void*)0           // element array buffer offset
        );

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }

    void cleanup() {
        glDeleteBuffers(1, &vertexBufferID);
        glDeleteBuffers(1, &colorBufferID);
        glDeleteBuffers(1, &indexBufferID);
        glDeleteVertexArrays(1, &vertexArrayID);
        glDeleteBuffers(1, &uvBufferID);
        glDeleteTextures(1, &textureID);
        glDeleteProgram(programID);
    }
};

struct Building {
    glm::vec3 position;		// Position of the box
    glm::vec3 scale;		// Size of the box in each axis
    void setTexture(GLuint tex) {
        textureID = tex;
    }
    GLfloat vertex_buffer_data[72] = {	// Vertex definition for a canonical box
            // Front face
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,

            // Back face
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,

            // Left face
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,

            // Right face
            1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,

            // Top face
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,

            // Bottom face
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,
    };

    GLfloat color_buffer_data[72] = {
            // Front, red
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,

            // Back, yellow
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,

            // Left, green
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,

            // Right, cyan
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,

            // Top, blue
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,

            // Bottom, magenta
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
    };

    GLuint index_buffer_data[36] = {		// 12 triangle faces of a box
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

            20, 21, 22,
            20, 22, 23,
    };

    // TODO: Define UV buffer data
    // ---------------------------
    // ---------------------------

    GLfloat uv_buffer_data[48] = {
            // Front
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,

            // Back
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            // Left
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            // Right
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,

            // Top - we do not want texture the top
            0.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 0.0f,
            // Bottom - we do not want texture the bottom
            0.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 0.0f,
    };

    // OpenGL buffers
    GLuint vertexArrayID;
    GLuint vertexBufferID;
    GLuint indexBufferID;
    GLuint colorBufferID;
    GLuint uvBufferID;
    GLuint textureID;

    // Shader variable IDs
    GLuint mvpMatrixID;
    GLuint textureSamplerID;
    GLuint programID;

    void initialize(glm::vec3 position, glm::vec3 scale) {
        // Define scale of the building geometry
        this->position = position;
        this->scale = scale;

        glClearColor(0.537f, 0.812f, 0.941f, 1.0f); // Set the background color to baby blue


        // Create a vertex array object
        glGenVertexArrays(1, &vertexArrayID);
        glBindVertexArray(vertexArrayID);

        // Create a vertex buffer object to store the vertex data
        glGenBuffers(1, &vertexBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

        // Create a vertex buffer object to store the color data
        // TODO:
        glGenBuffers(1, &colorBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data), color_buffer_data, GL_STATIC_DRAW);

        // TODO: Create a vertex buffer object to store the UV data
        for (int i = 0; i < 24; ++i) uv_buffer_data[2*i+1] *= 5;
        glGenBuffers(1, &uvBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data,
                     GL_STATIC_DRAW);

        // Create an index buffer object to store the index data that defines triangle faces
        glGenBuffers(1, &indexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_STATIC_DRAW);

        // Create and compile our GLSL program from the shaders
        programID = LoadShadersFromFile("../finalProject/box.vert", "../finalProject/box.frag");
        if (programID == 0)
        {
            std::cerr << "Failed to load shaders." << std::endl;
        }

        // Get a handle for our "MVP" uniform
        mvpMatrixID = glGetUniformLocation(programID, "MVP");

        // Get a handle to texture sampler
        textureSamplerID = glGetUniformLocation(programID,"textureSampler");
    }


    void render(glm::mat4 cameraMatrix) {
        glUseProgram(programID);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);


        // Enable UV buffer and texture sampler
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(textureSamplerID, 0);

        // Model transform
        glm::mat4 modelMatrix = glm::mat4();
        modelMatrix = glm::translate(modelMatrix, position); // Translate to the building's position
        modelMatrix = glm::scale(modelMatrix, scale);        // Scale the building

        // Set model-view-projection matrix
        glm::mat4 mvp = cameraMatrix * modelMatrix;
        glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);

        // Draw the box
        glDrawElements(
                GL_TRIANGLES,      // mode
                36,                // number of indices
                GL_UNSIGNED_INT,   // type
                (void*)0           // element array buffer offset
        );

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }

    void cleanup() {
        glDeleteBuffers(1, &vertexBufferID);
        glDeleteBuffers(1, &colorBufferID);
        glDeleteBuffers(1, &indexBufferID);
        glDeleteVertexArrays(1, &vertexArrayID);
        //glDeleteBuffers(1, &uvBufferID);
        //glDeleteTextures(1, &textureID);
        glDeleteProgram(programID);
    }
};

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
    window = glfwCreateWindow(1024, 768, "Final Project", NULL, NULL);
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

    // Load OpenGL functions, gladLoadGL returns the loaded version, 0 on error.
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        std::cerr << "Failed to initialize OpenGL context." << std::endl;
        return -1;
    }

    // Prepare shadow map size for shadow mapping. Usually this is the size of the window itself, but on some platforms like Mac this can be 2x the size of the window. Use glfwGetFramebufferSize to get the shadow map size properly.
    glfwGetFramebufferSize(window, &shadowMapWidth, &shadowMapHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Background
    glClearColor(0.7f, 0.5f, 0.5f, 1.0f);


    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    // TODO: Create more buildings
    // ---------------------------
    Skybox sky;
    sky.initialize(glm::vec3(eye_center.x, eye_center.y - 2500, eye_center.z), glm::vec3(5000, 5000, 5000),"C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\planet.jpeg");

    CornellBox b;
    b.initialize();
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    //Declare and initialize the textures vector
    std::vector<GLuint> textures;
    textures.push_back(LoadTextureTileBox("../finalProject/facade1.jpg"));
    textures.push_back(LoadTextureTileBox("../finalProject/facade2.jpg"));
    textures.push_back(LoadTextureTileBox("../finalProject/facade3.jpg"));
    textures.push_back(LoadTextureTileBox("../finalProject/facade4.jpg"));
    // Add more textures as needed
    // Create multiple buildings
    std::vector<Building> buildings;
    int numBuildings = 50; // Adjust this number to add more buildings

    for (int i = 0; i < numBuildings; ++i) {
        buildings.push_back(Building());
    }

    for (int i = 0; i < numBuildings; ++i) {
        float height = 1.0f + static_cast<float>(rand() % 10 + 1); // Random height between 1 and 10
        float xPos = static_cast<float>(rand() % 200 - 100); // Random x position between -20 and 20
        float zPos = static_cast<float>(rand() % 200 - 100); // Random z position between -20 and 20
        buildings[i].initialize(glm::vec3(xPos, 0.0f, zPos), glm::vec3(1.0f, height, 1.0f));
        buildings[i].setTexture(textures[i % textures.size()]);
    }
    // Camera setup
    eye_center.y = viewDistance * cos(viewPolar);
    eye_center.x = viewDistance * cos(viewAzimuth);
    eye_center.z = viewDistance * sin(viewAzimuth);

    glm::mat4 viewMatrix, projectionMatrix;
    glm::float32 FoV = 45.0f;
    glm::float32 zNear = 100.0f;
    glm::float32 zFar = 0.0f;
    projectionMatrix = glm::perspective(glm::radians(FoV), 1024.0f/768.0f, 10.0f, 10000.0f);

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
        // Render the building
        sky.render(vp);

        // Render the buildings
        for (auto& building : buildings) {
            building.render(vp);
        }

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (!glfwWindowShouldClose(window));

    // Clean up
    sky.cleanup();
    b.cleanup();
    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

// Is called whenever a key is pressed/released via GLFW
/*void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        viewAzimuth = 0.f;
        viewPolar = 0.f;
        eye_center.y = viewDistance * cos(viewPolar);
        eye_center.x = viewDistance * cos(viewAzimuth);
        eye_center.z = viewDistance * sin(viewAzimuth);
        std::cout << "Reset." << std::endl;
    }

    if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        viewPolar -= 0.1f;
        eye_center.y = viewDistance * cos(viewPolar);
    }

    if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        viewPolar += 0.1f;
        eye_center.y = viewDistance * cos(viewPolar);
    }

    if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        viewAzimuth -= 0.1f;
        eye_center.x = viewDistance * cos(viewAzimuth);
        eye_center.z = viewDistance * sin(viewAzimuth);
    }

    if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        viewAzimuth += 0.1f;
        eye_center.x = viewDistance * cos(viewAzimuth);
        eye_center.z = viewDistance * sin(viewAzimuth);
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}*/

/*void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        lookat = glm::vec3(0.0f, 0.0f, 0.0f);
        std::cout << "Reset." << std::endl;
    }

    if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        lookat.y += 1.0f;
    }

    if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        lookat.y -= 1.0f;
    }

    if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        lookat.x -= 1.0f;
    }

    if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        lookat.x += 1.0f;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}*/
void updateCameraPosition()
{
    eye_center.y = viewDistance * sin(viewPolar);
    eye_center.x = viewDistance * cos(viewPolar) * cos(viewAzimuth);
    eye_center.z = viewDistance * cos(viewPolar) * sin(viewAzimuth);
}

/*void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        viewAzimuth = glm::radians(45.0f);
        viewPolar = glm::radians(30.0f);
        updateCameraPosition();
        std::cout << "Reset." << std::endl;
    }

    if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        viewPolar -= 0.1f;
        updateCameraPosition();
    }

    if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        viewPolar += 0.1f;
        updateCameraPosition();
    }

    if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        viewAzimuth -= 0.1f;
        updateCameraPosition();
    }

    if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        viewAzimuth += 0.1f;
        updateCameraPosition();
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}*/



void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    glm::vec3 direction = glm::normalize(lookat - eye_center);
    glm::vec3 right = glm::normalize(glm::cross(direction, up));

    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        eye_center = glm::vec3(0.0f, 50.0f, 200.0f);
        lookat = glm::vec3(0.0f, 0.0f, 0.0f);
        std::cout << "Reset." << std::endl;
    }

    if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        eye_center += direction * 1.0f;
        lookat += direction * 1.0f;
    }

    if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        eye_center -= direction * 1.0f;
        lookat -= direction * 1.0f;
    }

    if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        eye_center -= right * 1.0f;
        lookat -= right * 1.0f;
    }

    if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        eye_center += right * 1.0f;
        lookat += right * 1.0f;
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
