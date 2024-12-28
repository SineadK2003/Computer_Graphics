cmake_minimum_required(VERSION 3.0)
project(lab2)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Set output directories for binaries and libraries
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

# Add external dependencies
add_subdirectory(external)

# Include directories
include_directories(
        external/glfw-3.1.2/include/
        external/glm-0.9.7.1/
        external/glad-3.3/include/
        external/
        lab2/
)

# Add glad source file
set(GLAD_SOURCES
        external/glad-3.3/src/gl.c
        )

# Add executable for lab2_building
add_executable(lab2_building
        lab2/lab2_building.cpp
        lab2/render/shader.cpp
        ${GLAD_SOURCES}  # Add glad source file
        )

# Link libraries for lab2_building
target_link_libraries(lab2_building
        ${OPENGL_LIBRARY}
        glfw
        )

# Add executable for lab2_skybox
add_executable(lab2_skybox
        lab2/lab2_skybox.cpp
        lab2/render/shader.cpp
        ${GLAD_SOURCES}  # Add glad source file
        )

# Link libraries for lab2_skybox
target_link_libraries(lab2_skybox
        ${OPENGL_LIBRARY}
        glfw
        )