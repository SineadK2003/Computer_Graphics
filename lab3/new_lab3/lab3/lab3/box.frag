#version 330 core

  in vec3 color;
  in vec3 worldPosition;
  in vec3 worldNormal;
  in vec4 lightSpacePos;

  out vec3 finalColor;

  uniform vec3 lightPosition;
  uniform vec3 lightIntensity;
  uniform vec3 ambientLight;

uniform sampler2D shadowMap;

float locateShadow(vec4 lightSpacePos) {
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;
    projCoords.xy = clamp(projCoords.xy, 0.0, 1.0);

    float depth = projCoords.z;

    if (depth > 1.0) return 1.0;

    float existingDepth = texture(shadowMap, projCoords.xy).r;

    return (depth >= existingDepth + 1e-2) ? 0.2 : 1.0;
}
  void main()
  {
      // Compute light direction and normalize
          vec3 lightDir = normalize(lightPosition - worldPosition);
          float distance = length(lightPosition - worldPosition);

          // Compute view direction (assuming the viewer is at the origin)
          vec3 viewDir = normalize(-worldPosition);

          // Compute the normal vector
          vec3 norm = normalize(worldNormal);
          vec3 halfway = normalize(lightDir + viewDir);

          // Diffuse lighting
          float cosTheta = max(dot(norm, lightDir), 0.0);

          // Specular lighting (Phong reflection model)
          vec3 reflectDir = reflect(-lightDir, norm);
          float spec = pow(max(dot(norm, halfway), 0.0), 32.0); // Shininess factor = 32

          // Compute shadow factor
          float shadow = locateShadow(lightSpacePos);

          // Combine all components
          vec3 diffuse = cosTheta * color * lightIntensity / (distance * distance) * shadow;
          vec3 specularColor = spec * lightIntensity / (distance * distance) * shadow;
          vec3 rawColor = diffuse + specularColor;

          // Tone mapping (Reinhard tone mapping)
          vec3 toneMappedColor = rawColor / (vec3(1.0) + rawColor);

          // Gamma correction
          float gamma = 2.2;
          finalColor = pow(toneMappedColor, vec3(1.0 / gamma));
  }