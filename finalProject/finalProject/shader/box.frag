#version 330 core

in vec2 uv;  // Input UV coordinate from vertex shader

out vec4 color;  // Output color

uniform sampler2D textureSampler;  // Texture sampler to access the texture

void main() {
    // Perform texture lookup using the UV coordinates
    vec3 finalColor = texture(textureSampler, uv).rgb;  // Fetch texture color based on UV coordinates

    // Optionally, apply the vertex color (if any) to the final color (you can remove vertex color if not needed)
    color = vec4(finalColor, 1.0);  // Set the output color with an alpha value of 1 (opaque)
}
