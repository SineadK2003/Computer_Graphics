cmake_minimum_required(VERSION 3.0)
project(finalProject)

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
        external/glew-2.1.0/include/  # Add GLEW include directory
        external/
        finalProject/
)

# Add glad source file
set(GLAD_SOURCES
        external/glad-3.3/src/gl.c
        )

set(GLEW_SOURCES
        external/glew-2.1.0/src/glew.c
        )


# Add executable for fp_building
add_executable(fp_building
        finalProject/fp_building.cpp
        finalProject/render/shader.cpp
        ${GLAD_SOURCES}  # Add glad source file
        ${GLEW_SOURCES}  # Add glew source file
        )

# Link libraries for fp_building
target_link_libraries(fp_building
        ${OPENGL_LIBRARY}
        glfw
        )

# Add executable for fp_skybox
add_executable(fp_skybox
        finalProject/fp_skybox.cpp
        finalProject/render/shader.cpp
        ${GLAD_SOURCES}  # Add glad source file
        ${GLEW_SOURCES}  # Add glew source file
        )

# Link libraries for fp_skybox
target_link_libraries(fp_skybox
        ${OPENGL_LIBRARY}
        glfw
        )

# Add executable for fp_skybox
add_executable(fp_main
        finalProject/fp_main.cpp
        finalProject/render/shader.cpp
        ${GLAD_SOURCES}  # Add glad source file
        ${GLEW_SOURCES}  # Add glew source file
        )

# Link libraries for fp_skybox
target_link_libraries(fp_main
        ${OPENGL_LIBRARY}
        glfw
        )