#version 330 core

layout(location = 0) in vec3 vertexPosition; // Position of the vertex
layout(location = 1) in vec3 vertexColor;    // Color for the vertex (optional if you're using textures)
layout(location = 2) in vec2 vertexUV;       // UV coordinates for texture mapping

uniform mat4 MVP;  // Model-View-Projection matrix

out vec2 uv;  // Output UV coordinate for the fragment shader

void main() {
    gl_Position = MVP * vec4(vertexPosition, 1.0);  // Apply transformation to vertex position
    uv = vertexUV;  // Pass UV coordinates to the fragment shader
}
