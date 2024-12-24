#version 330 core

  in vec3 color;
  in vec3 worldPosition;
  in vec3 worldNormal;

  out vec3 finalColor;

  uniform vec3 lightPosition;
  uniform vec3 lightIntensity;
  uniform vec3 ambientLight;

  void main()
  {
      // Normalize the normal vector
      vec3 normal = normalize(worldNormal);

      // Calculate the direction from the fragment to the light source
      vec3 lightDir = normalize(lightPosition - worldPosition);

      // Calculate the diffuse component using Lambert's cosine law
      float diff = max(dot(normal, lightDir), 0.0);

      // Calculate the diffuse color
      vec3 diffuse = diff * lightIntensity * color;

      // Calculate the ambient color
      vec3 ambient = ambientLight * color;

      // Ensure a minimum base color
      vec3 baseColor = vec3(0.2, 0.2, 0.2) * color;

      // Set the final color
      finalColor = max(baseColor, ambient + diffuse);
  }