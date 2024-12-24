#version 330 core

in vec3 color;
in vec3 worldPosition;
in vec3 worldNormal; 

out vec3 finalColor;

uniform vec3 lightPosition;
uniform vec3 lightIntensity;

void main()
{
	// Normalize the normal vector
         vec3 N = normalize(worldNormal);

         // Calculate the light direction vector
         vec3 L = normalize(lightPosition - worldPosition);

         // Calculate the diffuse component using Lambertian reflection
         float cosTheta = clamp(dot(N, L), 0.0, 1.0);
         vec3 diffuse = lightIntensity * color * cosTheta;

         // Attenuation
         float distance = length(lightPosition - worldPosition);
         float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
         diffuse *= attenuation;

         // Tone mapping
         vec3 mapped = diffuse / (diffuse + vec3(1.0));

         // Gamma correction
         finalColor = pow(mapped, vec3(1.0 / 2.2));
}
