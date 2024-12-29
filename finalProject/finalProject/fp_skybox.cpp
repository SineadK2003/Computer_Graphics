// Prevent tinygltf from including its own STB implementations
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE

// Include your STB implementations
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

// Include tinygltf
#define TINYGLTF_IMPLEMENTATION
#include <tinygltf-2.9.3/tiny_gltf.h>

// Other includes
//#include <GL/glew.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <render/shader.h>

#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iomanip>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

static GLFWwindow *window;
static int windowWidth = 1024;
static int windowHeight = 768;
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);


// Camera setup
glm::vec3 eye_center(0.0f, 0.0f, 0.0f); // Set the camera position higher and further away
glm::vec3 lookat(0.0f, 0.0f, -1.0f); // Look at the center of the scene
glm::vec3 up(0.0f, 1.0f, 0.0f); // Up vector
static float FoV = 45.0f;
static float zNear = 0.1f;
static float zFar = 10000.0f;

// Lighting control
const glm::vec3 wave500(0.0f, 255.0f, 146.0f);
const glm::vec3 wave600(255.0f, 190.0f, 0.0f);
const glm::vec3 wave700(205.0f, 0.0f, 0.0f);
static glm::vec3 lightIntensity = 5.0f * (8.0f * wave500 + 15.6f * wave600 + 18.4f * wave700);
static glm::vec3 lightPosition(-275.0f, 500.0f, -275.0f);

// View control
static float viewAzimuth = 0.f;
static float viewPolar = 0.f;
static float viewDistance = 200.0f;

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

// View Parameters
static float depthFoV = 90.0f;
static float depthNear = 90.0f;
static float depthFar = 1000.0f;

// Helper flag and function to save depth maps for debugging
static bool saveDepth = false;

// Animation
static bool playAnimation = true;
static float playbackSpeed = 2.0f;


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
        programID = LoadShadersFromFile("C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\shader\\skybox.vert", "C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\shader\\skybox.frag");
        if (programID == 0) {
            std::cerr << "Failed to load shaders." << std::endl;
        }

        // Get a handle for our "MVP" uniform
        mvpMatrixID = glGetUniformLocation(programID, "MVP");

        //  Load a texture
        textureID = LoadTextureTileBox(texturePath);

        // Get a handle to texture sampler
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
    glm::vec3 dimensions;
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

    glm::vec3 getPosition() const {
        return position;
    }

    glm::vec3 getSize() const {
        return dimensions;
    }

    void initialize(glm::vec3 position, glm::vec3 scale) {
        // Define scale of the building geometry
        this->position = position;
        this->scale = scale;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the background color to baby blue


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

        // Create a vertex buffer object to store the UV data
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
        programID = LoadShadersFromFile("C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\shader\\box.vert", "C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\shader\\box.frag");
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

struct Rocket {
    glm::vec3 position;		// Position of the box
    glm::vec3 scale;		// Size of the box in each axis
    glm::vec3 dimensions;
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

    GLfloat uv_buffer_data[48] = {
            // Front (+Z)
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,

            // Back (-Z)
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,

            // Left (+X)
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,

            // Right (-X)
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,

            // Top (+Y)
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,

            // Bottom (-Y)
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f
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

    glm::vec3 getPosition() const {
        return position;
    }

    glm::vec3 getSize() const {
        return dimensions;
    }

    void initialize(glm::vec3 position, glm::vec3 scale) {
        // Define scale of the building geometry
        this->position = position;
        this->scale = scale;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the background color to baby blue


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

        // Create a vertex buffer object to store the UV data
       // for (int i = 0; i < 24; ++i) uv_buffer_data[2*i+1] *= 5;
        glGenBuffers(1, &uvBufferID);
        glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(uv_buffer_data), uv_buffer_data,
                     GL_STATIC_DRAW);

        // Create an index buffer object to store the index data that defines triangle faces
        glGenBuffers(1, &indexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_STATIC_DRAW);

        // Create and compile our GLSL program from the shaders
        programID = LoadShadersFromFile("C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\shader\\box.vert", "C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\shader\\box.frag");
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

struct MyBot {
    // Shader variable IDs
    GLuint mvpMatrixID;
    GLuint jointMatricesID;
    GLuint lightPositionID;
    GLuint lightIntensityID;
    GLuint programID;

    tinygltf::Model model;

    // Each VAO corresponds to each mesh primitive in the GLTF model
    struct PrimitiveObject {
        GLuint vao;
        std::map<int, GLuint> vbos;
    };
    std::vector<PrimitiveObject> primitiveObjects;

    // Skinning
    struct SkinObject {
        // Transforms the geometry into the space of the respective joint
        std::vector<glm::mat4> inverseBindMatrices;

        // Transforms the geometry following the movement of the joints
        std::vector<glm::mat4> globalJointTransforms;

        // Combined transforms
        std::vector<glm::mat4> jointMatrices;
    };
    std::vector<SkinObject> skinObjects;

    // Animation
    struct SamplerObject {
        std::vector<float> input;
        std::vector<glm::vec3> outputVec3;
        std::vector<glm::vec4> outputVec4;
        int interpolation;
    };

    struct NodeState {
        glm::vec3 translation;
        glm::quat rotation;
        glm::vec3 scale;
    };

    std::vector<NodeState> nodeStates;

    struct ChannelObject {
        int sampler;
        std::string targetPath;
        int targetNode;
    };
    struct AnimationObject {
        std::vector<SamplerObject> samplers;	// Animation data
    };

    std::vector<AnimationObject> animationObjects;

    glm::mat4 getNodeTransform(const tinygltf::Node& node) {
        glm::mat4 transform(1.0f);

        if (node.matrix.size() == 16) {
            transform = glm::make_mat4(node.matrix.data());
        } else {
            if (node.translation.size() == 3) {
                transform = glm::translate(transform, glm::vec3(node.translation[0], node.translation[1], node.translation[2]));
            }
            if (node.rotation.size() == 4) {
                glm::quat q(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
                transform *= glm::mat4_cast(q);
            }
            if (node.scale.size() == 3) {
                transform = glm::scale(transform, glm::vec3(node.scale[0], node.scale[1], node.scale[2]));
            }
        }
        return transform;
    }

    void computeLocalNodeTransform(const tinygltf::Model& model,
                                   int nodeIndex,
                                   std::vector<glm::mat4> &localTransforms)
    {
        const tinygltf::Node &node = model.nodes[nodeIndex];

        // get local transform for the current node
        localTransforms[nodeIndex] = getNodeTransform(node);

        // recursively compute transforms for all child nodes
        for (int childIndex : node.children) {
            computeLocalNodeTransform(model, childIndex, localTransforms);
        }
    }

    void computeGlobalNodeTransform(const tinygltf::Model& model,
                                    const std::vector<glm::mat4> &localTransforms,
                                    int nodeIndex, const glm::mat4& parentTransform,
                                    std::vector<glm::mat4> &globalTransforms)
    {
        // compute global transform for the current node
        globalTransforms[nodeIndex] = parentTransform * localTransforms[nodeIndex];

        // recursively compute global transforms for child nodes
        const tinygltf::Node &node = model.nodes[nodeIndex];
        for (int childIndex : node.children) {
            computeGlobalNodeTransform(model, localTransforms, childIndex, globalTransforms[nodeIndex], globalTransforms);
        }
    }

    std::vector<SkinObject> prepareSkinning(const tinygltf::Model &model) {
        std::vector<SkinObject> skinObjects;

        // In our Blender exporter, the default number of joints that may influence a vertex is set to 4, just for convenient implementation in shaders.

        for (size_t i = 0; i < model.skins.size(); i++) {
            SkinObject skinObject;

            const tinygltf::Skin &skin = model.skins[i];

            // Read inverseBindMatrices
            const tinygltf::Accessor &accessor = model.accessors[skin.inverseBindMatrices];
            assert(accessor.type == TINYGLTF_TYPE_MAT4);
            const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
            const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
            const float *ptr = reinterpret_cast<const float *>(
                    buffer.data.data() + accessor.byteOffset + bufferView.byteOffset);

            skinObject.inverseBindMatrices.resize(accessor.count);
            for (size_t j = 0; j < accessor.count; j++) {
                float m[16];
                memcpy(m, ptr + j * 16, 16 * sizeof(float));
                skinObject.inverseBindMatrices[j] = glm::make_mat4(m);
            }

            assert(skin.joints.size() == accessor.count);

            skinObject.globalJointTransforms.resize(skin.joints.size());
            skinObject.jointMatrices.resize(skin.joints.size());

            // Compute local transforms for all nodes
            std::vector<glm::mat4> localTransforms(model.nodes.size(), glm::mat4(1.0f));
            for (size_t i = 0; i < model.nodes.size(); ++i) {
                localTransforms[i] = getNodeTransform(model.nodes[i]);
            }

            // Compute global transforms starting from the scene's root nodes
            std::vector<glm::mat4> globalTransforms(model.nodes.size(), glm::mat4(1.0f));
            for (size_t i = 0; i < model.scenes[model.defaultScene].nodes.size(); ++i) {
                computeGlobalNodeTransform(model, localTransforms, model.scenes[model.defaultScene].nodes[i], glm::mat4(1.0f), globalTransforms);
            }

            // Store global joint transforms
            skinObject.globalJointTransforms.resize(model.nodes.size(), glm::mat4(1.0f));
            for (size_t j = 0; j < skin.joints.size(); ++j) {
                int nodeIndex = skin.joints[j];
                skinObject.globalJointTransforms[nodeIndex] = globalTransforms[nodeIndex];
            }
            skinObjects.push_back(skinObject);
        }

        return skinObjects;
    }

    int findKeyframeIndex(const std::vector<float>& times, float animationTime)
    {
        int left = 0;
        int right = times.size() - 1;

        while (left <= right) {
            int mid = (left + right) / 2;

            if (mid + 1 < times.size() && times[mid] <= animationTime && animationTime < times[mid + 1]) {
                return mid;
            }
            else if (times[mid] > animationTime) {
                right = mid - 1;
            }
            else { // animationTime >= times[mid + 1]
                left = mid + 1;
            }
        }

        // Target not found
        return times.size() - 2;
    }

    std::vector<AnimationObject> prepareAnimation(const tinygltf::Model &model)
    {
        std::vector<AnimationObject> animationObjects;
        for (const auto &anim : model.animations) {
            AnimationObject animationObject;

            for (const auto &sampler : anim.samplers) {
                SamplerObject samplerObject;

                // Read input times
                const tinygltf::Accessor &inputAccessor = model.accessors[sampler.input];
                const tinygltf::BufferView &inputBufferView = model.bufferViews[inputAccessor.bufferView];
                const tinygltf::Buffer &inputBuffer = model.buffers[inputBufferView.buffer];

                const unsigned char *inputPtr = &inputBuffer.data[inputBufferView.byteOffset + inputAccessor.byteOffset];
                int inputStride = inputAccessor.ByteStride(inputBufferView);

                samplerObject.input.resize(inputAccessor.count);
                for (size_t i = 0; i < inputAccessor.count; ++i) {
                    const float *p = reinterpret_cast<const float*>(inputPtr + i * inputStride);
                    samplerObject.input[i] = *p;
                }

                // Read output values
                const tinygltf::Accessor &outputAccessor = model.accessors[sampler.output];
                const tinygltf::BufferView &outputBufferView = model.bufferViews[outputAccessor.bufferView];
                const tinygltf::Buffer &outputBuffer = model.buffers[outputBufferView.buffer];

                const unsigned char *outputPtr = &outputBuffer.data[outputBufferView.byteOffset + outputAccessor.byteOffset];
                int outputStride = outputAccessor.ByteStride(outputBufferView);

                if (outputAccessor.type == TINYGLTF_TYPE_VEC3) {
                    // VEC3 data (translation or scale)
                    samplerObject.outputVec3.resize(outputAccessor.count);
                    for (size_t i = 0; i < outputAccessor.count; ++i) {
                        const float *p = reinterpret_cast<const float*>(outputPtr + i * outputStride);
                        samplerObject.outputVec3[i] = glm::vec3(p[0], p[1], p[2]);
                    }
                } else if (outputAccessor.type == TINYGLTF_TYPE_VEC4) {
                    // VEC4 data (rotation)
                    samplerObject.outputVec4.resize(outputAccessor.count);
                    for (size_t i = 0; i < outputAccessor.count; ++i) {
                        const float *p = reinterpret_cast<const float*>(outputPtr + i * outputStride);
                        samplerObject.outputVec4[i] = glm::vec4(p[0], p[1], p[2], p[3]);
                    }
                } else {
                    std::cout << "Unsupported accessor type in animation output" << std::endl;
                }

                animationObject.samplers.push_back(samplerObject);
            }

            animationObjects.push_back(animationObject);
        }
        return animationObjects;
    }


    void updateAnimation(
            const tinygltf::Model &model,
            const tinygltf::Animation &anim,
            const AnimationObject &animationObject,
            float time)
    {
        for (size_t i = 0; i < anim.channels.size(); ++i) {
            const auto &channel = anim.channels[i];
            int targetNodeIndex = channel.target_node;
            const auto &sampler = anim.samplers[channel.sampler];

            const SamplerObject &samplerObject = animationObject.samplers[channel.sampler];

            const std::vector<float> &times = samplerObject.input;
            float animationTime = fmod(time, times.back());

            int keyframeIndex = findKeyframeIndex(times, animationTime);
            float t = (animationTime - times[keyframeIndex]) /
                      (times[keyframeIndex + 1] - times[keyframeIndex]);

            NodeState &state = nodeStates[targetNodeIndex];

            if (channel.target_path == "translation") {
                glm::vec3 translation0 = samplerObject.outputVec3[keyframeIndex];
                glm::vec3 translation1 = samplerObject.outputVec3[keyframeIndex + 1];

                glm::vec3 translation = glm::mix(translation0, translation1, t);
                state.translation = translation;
            } else if (channel.target_path == "rotation") {
                glm::quat rotation0 = glm::quat(samplerObject.outputVec4[keyframeIndex].w, samplerObject.outputVec4[keyframeIndex].x, samplerObject.outputVec4[keyframeIndex].y, samplerObject.outputVec4[keyframeIndex].z);
                glm::quat rotation1 = glm::quat(samplerObject.outputVec4[keyframeIndex + 1].w, samplerObject.outputVec4[keyframeIndex + 1].x, samplerObject.outputVec4[keyframeIndex + 1].y, samplerObject.outputVec4[keyframeIndex + 1].z);

                glm::quat rotation = glm::slerp(rotation0, rotation1, t);
                state.rotation = rotation;
            } else if (channel.target_path == "scale") {
                glm::vec3 scale0 = samplerObject.outputVec3[keyframeIndex];
                glm::vec3 scale1 = samplerObject.outputVec3[keyframeIndex + 1];

                glm::vec3 scale = glm::mix(scale0, scale1, t);
                state.scale = scale;
            }
        }
    }


    void updateSkinning(const std::vector<glm::mat4> &globalTransforms) {
        for (auto &skinObject : skinObjects) {
            for (size_t j = 0; j < skinObject.jointMatrices.size(); ++j) {
                int nodeIndex = model.skins[0].joints[j];  // Assuming single skin
                skinObject.jointMatrices[j] = globalTransforms[nodeIndex] * skinObject.inverseBindMatrices[j];
            }
        }
    }


    void update(float time) {
        if (model.animations.size() > 0) {
            const tinygltf::Animation &animation = model.animations[0];
            const AnimationObject &animationObject = animationObjects[0];

            // Apply animations to nodeStates
            updateAnimation(model, animation, animationObject, time);

            // Reconstruct nodeTransforms from nodeStates
            std::vector<glm::mat4> nodeTransforms(model.nodes.size(), glm::mat4(1.0f));
            for (size_t i = 0; i < model.nodes.size(); ++i) {
                NodeState &state = nodeStates[i];
                glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), state.translation);
                glm::mat4 rotationMatrix = glm::mat4_cast(state.rotation);
                glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), state.scale);

                nodeTransforms[i] = translationMatrix * rotationMatrix * scaleMatrix;
            }

            // Compute global transforms
            std::vector<glm::mat4> globalTransforms(model.nodes.size(), glm::mat4(1.0f));
            for (size_t i = 0; i < model.scenes[model.defaultScene].nodes.size(); ++i) {
                computeGlobalNodeTransform(model, nodeTransforms,
                                           model.scenes[model.defaultScene].nodes[i], glm::mat4(1.0f), globalTransforms);
            }

            // Recompute joint matrices for skinning
            updateSkinning(globalTransforms);
        }
    }


    bool loadModel(tinygltf::Model &model, const char *filename) {
        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;

        bool res = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
        if (!warn.empty()) {
            std::cout << "WARN: " << warn << std::endl;
        }

        if (!err.empty()) {
            std::cout << "ERR: " << err << std::endl;
        }

        if (!res)
            std::cout << "Failed to load glTF: " << filename << std::endl;
        else
            std::cout << "Loaded glTF: " << filename << std::endl;

        return res;
    }

    void initialize(const glm::vec3& pos) {
        position = pos;
        if (!loadModel(model, "C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\model\\bot\\bot.gltf")) {
            return;
        }

        // Apply scaling transformation
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)); // Adjust the scale factor as needed
        for (auto& node : model.nodes) {
            if (node.matrix.size() == 16) {
                glm::mat4 nodeMatrix = glm::make_mat4(node.matrix.data());
                nodeMatrix = scaleMatrix * nodeMatrix;
                std::copy(glm::value_ptr(nodeMatrix), glm::value_ptr(nodeMatrix) + 16, node.matrix.begin());
            } else {
                // Apply scaling to translation, rotation, and scale if matrix is not present
                if (node.translation.size() == 3) {
                    node.translation[0] *= 0.1f;
                    node.translation[1] *= 0.1f;
                    node.translation[2] *= 0.1f;
                }
                if (node.scale.size() == 3) {
                    node.scale[0] *= 0.1f;
                    node.scale[1] *= 0.1f;
                    node.scale[2] *= 0.1f;
                }
            }
        }

        // Prepare buffers for rendering
        primitiveObjects = bindModel(model);

        // Prepare joint matrices
        skinObjects = prepareSkinning(model);

        // Prepare animation data
        animationObjects = prepareAnimation(model);

        // Create and compile our GLSL program from the shaders
        programID = LoadShadersFromFile("C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\shader\\bot.vert", "C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\shader\\bot.frag");
        if (programID == 0) {
            std::cerr << "Failed to load shaders." << std::endl;
        }

        nodeStates.resize(model.nodes.size());
        for (size_t i = 0; i < model.nodes.size(); ++i) {
            const tinygltf::Node &node = model.nodes[i];

            NodeState &state = nodeStates[i];
            // Translation
            if (node.translation.size() == 3) {
                state.translation = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
            } else {
                state.translation = glm::vec3(0.0f);
            }
            // Rotation
            if (node.rotation.size() == 4) {
                state.rotation = glm::quat(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
            } else {
                state.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            }
            // Scale
            if (node.scale.size() == 3) {
                state.scale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
            } else {
                state.scale = glm::vec3(1.0f);
            }
        }

        // Get a handle for GLSL variables
        mvpMatrixID = glGetUniformLocation(programID, "MVP");
        lightPositionID = glGetUniformLocation(programID, "lightPosition");
        lightIntensityID = glGetUniformLocation(programID, "lightIntensity");
        jointMatricesID = glGetUniformLocation(programID, "jointMatrices");
    }

    void bindMesh(std::vector<PrimitiveObject> &primitiveObjects,
                  tinygltf::Model &model, tinygltf::Mesh &mesh) {

        std::map<int, GLuint> vbos;
        for (size_t i = 0; i < model.bufferViews.size(); ++i) {
            const tinygltf::BufferView &bufferView = model.bufferViews[i];

            int target = bufferView.target;

            if (bufferView.target == 0) {
                // The bufferView with target == 0 in our model refers to
                // the skinning weights, for 25 joints, each 4x4 matrix (16 floats), totaling to 400 floats or 1600 bytes.
                // So it is considered safe to skip the warning.
                //std::cout << "WARN: bufferView.target is zero" << std::endl;
                continue;
            }

            const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
            GLuint vbo;
            glGenBuffers(1, &vbo);
            glBindBuffer(target, vbo);
            glBufferData(target, bufferView.byteLength,
                         &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);

            vbos[i] = vbo;
        }

        // Each mesh can contain several primitives (or parts), each we need to
        // bind to an OpenGL vertex array object
        for (size_t i = 0; i < mesh.primitives.size(); ++i) {

            tinygltf::Primitive primitive = mesh.primitives[i];
            tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

            GLuint vao;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            for (auto &attrib : primitive.attributes) {
                tinygltf::Accessor accessor = model.accessors[attrib.second];
                int byteStride =
                        accessor.ByteStride(model.bufferViews[accessor.bufferView]);
                glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

                int size = 1;
                if (accessor.type != TINYGLTF_TYPE_SCALAR) {
                    size = accessor.type;
                }

                int vaa = -1;
                if (attrib.first == "POSITION") vaa = 0;
                else if (attrib.first == "NORMAL") vaa = 1;
                else if (attrib.first == "TEXCOORD_0") vaa = 2;
                else if (attrib.first == "JOINTS_0") vaa = 3;
                else if (attrib.first == "WEIGHTS_0") vaa = 4;

                if (vaa > -1) {
                    glEnableVertexAttribArray(vaa);

                    if (attrib.first == "JOINTS_0") {
                        // integer attribute pointer for joint indices
                        glVertexAttribIPointer(vaa, size, accessor.componentType,
                                               byteStride, BUFFER_OFFSET(accessor.byteOffset));
                    } else {
                        // regular attribute pointer for other attributes
                        glVertexAttribPointer(vaa, size, accessor.componentType,
                                              accessor.normalized ? GL_TRUE : GL_FALSE,
                                              byteStride, BUFFER_OFFSET(accessor.byteOffset));
                    }
                } else {
                    std::cout << "Unknown attribute: " << attrib.first << std::endl;
                }
            }

            PrimitiveObject primitiveObject;
            primitiveObject.vao = vao;
            primitiveObject.vbos = vbos;
            primitiveObjects.push_back(primitiveObject);

            glBindVertexArray(0);
        }
    }

    void bindModelNodes(std::vector<PrimitiveObject> &primitiveObjects,
                        tinygltf::Model &model,
                        tinygltf::Node &node) {
        // Bind buffers for the current mesh at the node
        if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
            bindMesh(primitiveObjects, model, model.meshes[node.mesh]);
        }

        // Recursive into children nodes
        for (size_t i = 0; i < node.children.size(); i++) {
            assert((node.children[i] >= 0) && (node.children[i] < model.nodes.size()));
            bindModelNodes(primitiveObjects, model, model.nodes[node.children[i]]);
        }
    }

    std::vector<PrimitiveObject> bindModel(tinygltf::Model &model) {
        std::vector<PrimitiveObject> primitiveObjects;

        const tinygltf::Scene &scene = model.scenes[model.defaultScene];
        for (size_t i = 0; i < scene.nodes.size(); ++i) {
            assert((scene.nodes[i] >= 0) && (scene.nodes[i] < model.nodes.size()));
            bindModelNodes(primitiveObjects, model, model.nodes[scene.nodes[i]]);
        }

        return primitiveObjects;
    }

    void drawMesh(const std::vector<PrimitiveObject> &primitiveObjects,
                  tinygltf::Model &model, tinygltf::Mesh &mesh) {

        for (size_t i = 0; i < mesh.primitives.size(); ++i)
        {
            GLuint vao = primitiveObjects[i].vao;
            std::map<int, GLuint> vbos = primitiveObjects[i].vbos;

            glBindVertexArray(vao);

            tinygltf::Primitive primitive = mesh.primitives[i];
            tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos.at(indexAccessor.bufferView));

            glDrawElements(primitive.mode, indexAccessor.count,
                           indexAccessor.componentType,
                           BUFFER_OFFSET(indexAccessor.byteOffset));

            glBindVertexArray(0);
        }
    }

    void drawModelNodes(const std::vector<PrimitiveObject>& primitiveObjects,
                        tinygltf::Model &model, tinygltf::Node &node) {
        // Draw the mesh at the node, and recursively do so for children nodes
        if ((node.mesh >= 0) && (node.mesh < model.meshes.size())) {
            drawMesh(primitiveObjects, model, model.meshes[node.mesh]);
        }
        for (size_t i = 0; i < node.children.size(); i++) {
            drawModelNodes(primitiveObjects, model, model.nodes[node.children[i]]);
        }
    }
    void drawModel(const std::vector<PrimitiveObject>& primitiveObjects,
                   tinygltf::Model &model) {
        // Draw all nodes
        const tinygltf::Scene &scene = model.scenes[model.defaultScene];
        for (size_t i = 0; i < scene.nodes.size(); ++i) {
            drawModelNodes(primitiveObjects, model, model.nodes[scene.nodes[i]]);
        }
    }

    void render(glm::mat4 cameraMatrix) {
        glUseProgram(programID);

        // Set camera
        glm::mat4 mvp = cameraMatrix;
        glUniformMatrix4fv(mvpMatrixID, 1, GL_FALSE, &mvp[0][0]);

        // Set light data
        glUniform3fv(lightPositionID, 1, &lightPosition[0]);
        glUniform3fv(lightIntensityID, 1, &lightIntensity[0]);

        for (const auto &skinObject : skinObjects) {
            glUniformMatrix4fv(jointMatricesID, skinObject.jointMatrices.size(), GL_FALSE,
                               glm::value_ptr(skinObject.jointMatrices[0]));
        }

        // Draw the GLTF model
        drawModel(primitiveObjects, model);
    }

    void cleanup() {
        glDeleteProgram(programID);
    }

    glm::vec3 position;
};

bool isPositionInBuilding(const glm::vec3& position, const glm::vec3& size, const std::vector<Building>& buildings, float buffer) {
    for (const auto& building : buildings) {
        glm::vec3 buildingPos = building.getPosition();
        glm::vec3 buildingSize = building.getSize();
        if (position.x < buildingPos.x + buildingSize.x + buffer && position.x + size.x + buffer > buildingPos.x &&
            position.z < buildingPos.z + buildingSize.z + buffer && position.z + size.z + buffer > buildingPos.z) {
            return true;
        }
    }
    return false;
}

int main(void) {

    // Initialise GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // For MacOS
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "Final Project", NULL, NULL);
    if (window == NULL) {
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
    if (version == 0) {
        std::cerr << "Failed to initialize OpenGL context." << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    Skybox sky;
    sky.initialize(glm::vec3(eye_center.x, eye_center.y - 5000, eye_center.z), glm::vec3(5000, 5000, 5000),
                   "C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\background\\planet8.jpeg");


    // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    //Declare and initialize the textures vector
    std::vector<GLuint> textures;
    textures.push_back(LoadTextureTileBox(
            "C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\buildings\\facade6.jpg"));
    textures.push_back(LoadTextureTileBox(
            "C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\buildings\\facade7.jpg"));
    textures.push_back(LoadTextureTileBox(
            "C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\buildings\\facade8.jpg"));
    textures.push_back(LoadTextureTileBox(
            "C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\buildings\\facade9.jpg"));
    // Add more textures as needed

    // Create multiple buildings
    std::vector<Building> buildings;
    int numBuildings = 200; // Adjust this number to add more buildings

    for (int i = 0; i < numBuildings; ++i) {
        buildings.push_back(Building());
    }

    float buffer = 40.0f; // Adjust this value to increase or decrease the buffer zone

    for (int i = 0; i < numBuildings; ++i) {
        // Increase the size range for buildings
        float height = 10.0f + static_cast<float>(rand() % 100 + 1); // Random height between 10 and 110
        float width = 10.0f + static_cast<float>(rand() % 20 + 1);   // Random width between 10 and 30
        float depth = 10.0f + static_cast<float>(rand() % 20 + 1);   // Random depth between 10 and 30

        glm::vec3 size(width, height, depth);
        glm::vec3 position;

        // Ensure buildings do not overlap
        do {
            position.x = static_cast<float>(rand() % 2000 - 1000); // Random x position between -1000 and 1000
            position.z = static_cast<float>(rand() % 2000 - 1000); // Random z position between -1000 and 1000
        } while (isPositionInBuilding(position, size, buildings, buffer));

        buildings[i].initialize(position, size);
        buildings[i].setTexture(textures[i % textures.size()]);
    }


    /*std::vector<GLuint> treeTextures;
    treeTextures.push_back(LoadTextureTileBox(
            "C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\buildings\\tree.jpeg"));

    std::vector<Building> trees;
    int numTrees = 20; // Adjust this number to add more buildings

    for (int i = 0; i < numTrees; ++i) {
        trees.push_back(Building());
    }

    for (int i = 0; i < numTrees; ++i) {
        // Increase the size range for buildings
        // Increase the size range for buildings
        float height_t = 25.0f ; // Random height between 10 and 110
        float width_t = 25.0f;   // Random width between 10 and 30
        float depth_t = 25.0f;   // Random depth between 10 and 30

        glm::vec3 size_t(width_t, height_t, depth_t);
        glm::vec3 position_t;

        // Ensure trees do not overlap with buildings
        do {
            position_t.x = static_cast<float>(rand() % 2000 - 1000); // Random x position between -1000 and 1000
            position_t.z = static_cast<float>(rand() % 2000 - 1000); // Random z position between -1000 and 1000
        } while (isPositionInBuilding(position_t, size_t, buildings, buffer));

        trees[i].initialize(position_t, size_t);
        trees[i].setTexture(treeTextures[i % treeTextures.size()]);
        std::cout << "Tree initialized at position: (" << position_t.x << ", 0, " << position_t.z << ") with size: (" << width_t << ", " << height_t << ", " << depth_t << ")" << std::endl;

}*/

    std::vector<GLuint> rocketTextures;
    rocketTextures.push_back(LoadTextureTileBox(
            "C:\\Computer_Graphics_Git\\Computer_Graphics\\finalProject\\finalProject\\buildings\\rocket.jpeg"));

    std::vector<Rocket> rockets;
    int numRockets = 20; // Adjust this number to add more buildings

    for (int i = 0; i < numRockets; ++i) {
        rockets.push_back(Rocket());
    }

    for (int i = 0; i < numRockets; ++i) {
        // Increase the size range for buildings
        // Increase the size range for buildings
        float height_r = 25.0f ; // Random height between 10 and 110
        float width_r = 25.0f;   // Random width between 10 and 30
        float depth_r = 25.0f;   // Random depth between 10 and 30

        glm::vec3 size_t(width_r, height_r, depth_r);
        glm::vec3 position_r;

        // Ensure trees do not overlap with buildings
        do {
            position_r.x = static_cast<float>(rand() % 2000 - 1000); // Random x position between -1000 and 1000
            position_r.z = static_cast<float>(rand() % 2000 - 1000); // Random z position between -1000 and 1000
            position_r.y = static_cast<float>(rand() % 700 + 300);   // Random y position between 100 and 600 (sky level)
        } while (isPositionInBuilding(position_r, size_t, buildings, buffer));

        rockets[i].initialize(position_r, size_t);
        rockets[i].setTexture(rocketTextures[i % rocketTextures.size()]);
}


    /*// Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    // Create multiple bots
    std::vector<MyBot> bots;
    int numBots = 20; // Adjust this number to add more bots

    for (int i = 0; i < numBots; ++i) {
        bots.push_back(MyBot());
    }

    for (int i = 0; i < numBots; ++i) {

        // Increase the range for position generation
        float xPos = static_cast<float>(rand() % 2000 - 1000); // Random x position between -1000 and 1000
        float zPos = static_cast<float>(rand() % 2000 - 1000); // Random z position between -1000 and 1000

        bots[i].initialize(glm::vec3(xPos, 0.0f, zPos));

    }*/

    // Create a single bot
    MyBot bot;
    float botXPos = static_cast<float>(rand() % 2000 - 1000); // Random x position between -1000 and 1000
    float botZPos = static_cast<float>(rand() % 2000 - 1000); // Random z position between -1000 and 1000
    bot.initialize(glm::vec3(botXPos, 0.0f, botZPos));

// Camera setup
    eye_center.y = viewDistance * cos(viewPolar);
    eye_center.x = viewDistance * cos(viewAzimuth);
    eye_center.z = viewDistance * sin(viewAzimuth);

    glm::mat4 viewMatrix, projectionMatrix;
    float FoV = 45.0f;
    float zNear = 10.0f;
    float zFar = 10000.0f;
    projectionMatrix = glm::perspective(glm::radians(FoV), 1024.0f / 768.0f, zNear, zFar);

// Time and frame rate tracking
    static double lastTime = glfwGetTime();
    float time = 0.0f; // Animation time
    float fTime = 0.0f; // Time for measuring fps
    unsigned long frames = 0;


// Main loop
    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Disable face culling
        glDisable(GL_CULL_FACE);

        // Enable depth testing
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // Update states for animation
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        if (playAnimation) {
            time += deltaTime * playbackSpeed;

                bot.update(time);

        }

        // Rendering
        viewMatrix = glm::lookAt(eye_center, lookat, up);
        glm::mat4 vp = projectionMatrix * viewMatrix;

        // Render the skybox
        sky.render(vp);

        // Render the buildings
        for (auto &building : buildings) {
            building.render(vp);
        }
        // Render the buildings
       /* for (auto &tree: trees) {
            tree.render(vp);
        }*/

        for (auto &rocket: rockets) {
            rocket.render(vp);
        }


        /*// Render the bots
        int i = 0;
        for (auto& bot : bots) {
            i++;
            std::cout << "Rendering bot number: (" << i << std::endl;
            bot.render(vp);

            std::cout << "Rendering bot at position: (" << bot.position.x << ", " << bot.position.y << ", " << bot.position.z << ")" << std::endl;
        }*/

        // Render the single bot
        bot.render(vp);

        // FPS tracking
        frames++;
        fTime += deltaTime;
        if (fTime > 2.0f) {
            float fps = frames / fTime;
            frames = 0;
            fTime = 0;

            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << "Final Project | Frames per second (FPS): " << fps;
            glfwSetWindowTitle(window, stream.str().c_str());
        }

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (!glfwWindowShouldClose(window));

// Clean up
    sky.cleanup();

        bot.cleanup();

// Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}



void updateCameraPosition()
{
    eye_center.y = viewDistance * sin(viewPolar);
    eye_center.x = viewDistance * cos(viewPolar) * cos(viewAzimuth);
    eye_center.z = viewDistance * cos(viewPolar) * sin(viewAzimuth);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    float moveSpeed = 10.0f; // Increase this value to speed up the camera movement
    float verticalBoundary = 1500.0f; // Define the vertical boundary for the camera

    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        viewAzimuth = 0.f;
        viewPolar = 0.f;
        eye_center.y = viewDistance * cos(viewPolar);
        eye_center.x = viewDistance * cos(viewAzimuth);
        eye_center.z = viewDistance * sin(viewAzimuth);
        std::cout << "Reset." << std::endl;
    }

    if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        lookat.y += moveSpeed;
        if (lookat.y > verticalBoundary) {
            lookat.y = -verticalBoundary; // Loop back to the bottom
        }
    }


    if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        lookat.y -= moveSpeed;
        if (lookat.y < -verticalBoundary) {
            lookat.y = verticalBoundary; // Loop back to the top
        }
    }

    if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        viewAzimuth -= 0.1f;
        eye_center.x = viewDistance * cos(viewAzimuth);
        eye_center.z = viewDistance * sin(viewAzimuth);
    }

    if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        viewAzimuth += 0.1f;
        eye_center.x = viewDistance * cos(viewAzimuth);
        eye_center.z = viewDistance * sin(viewAzimuth);
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static float lastX = 1024.0f / 2.0;
    static float lastY = 768.0f / 2.0;
    static float yaw = -90.0f;
    static float pitch = 0.0f;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    lookat = glm::normalize(front);
}