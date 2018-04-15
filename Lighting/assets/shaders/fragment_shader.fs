#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;  
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 diffuse_color;
uniform vec3 ambient_strength;
uniform vec3 specular_strength;

void main()
{
    // ambient
    vec3 ambient = lightColor * ambient_strength;
  	
 	// diffuse 
    vec3 norm = (normalize(Normal));
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * diffuse_color);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = lightColor * (spec * specular_strength);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result * vec3(texture(texture_diffuse1, TexCoords)), 1.0);
}
