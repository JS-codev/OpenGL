#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform vec3 lightPos;

struct Material {
    sampler2D diffuse;   // Changed from vec3 to sampler2D
    sampler2D specular;  // Changed from vec3 to sampler2D
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;

void main() {
    // Sample textures
    vec3 diffuseColor = texture(material.diffuse, TexCoord).rgb;
    vec3 specularColor = texture(material.specular, TexCoord).rgb;
    
    // Ambient lighting
    vec3 ambient = light.ambient * diffuseColor;
    
    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffuseColor;
    
    // Specular lighting (Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specularColor;
    
    // Combine all lighting components
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}