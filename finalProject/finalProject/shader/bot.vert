#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in uvec4 joints;
layout(location = 4) in vec4 weights;

out vec3 worldPosition;
out vec3 worldNormal;

uniform mat4 MVP;
uniform mat4 jointMatrices[100];  
uniform vec3 lightPosition;

void main() {
    // Initialise transformed position and normal
    vec4 skinnedPosition = vec4(0.0);
    vec3 skinnedNormal = vec3(0.0);
    
    // Calculate skinning transform
    for(int i = 0; i < 4; i++) {
        uint jointIndex = joints[i];
        float weight = weights[i];
        
        // Skip if weight is zero
        if(weight > 0.0){
        
        // Transform position and normal by joint matrix
        mat4 jointTransform = jointMatrices[jointIndex];
        skinnedPosition += (jointTransform * vec4(vertexPosition, 1.0)) * weight;
        skinnedNormal += (mat3(jointTransform) * vertexNormal) * weight;
        }

    }
    
    // Output final position and normal
    gl_Position = MVP * skinnedPosition;
    worldPosition = vec3(skinnedPosition);
    worldNormal = normalize(skinnedNormal);
}