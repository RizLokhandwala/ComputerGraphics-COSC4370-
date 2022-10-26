// Resources 
    // "Learn OpenGL - Graphics Programming" - Joey de Vries

#version 330 core
out vec4 color;

in vec3 FragPos;  
in vec3 Normal;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // TODO: Replace with your code...
    
    
    float ambience_strength = 0.1;
    vec3 ambient_lighting = ambience_strength * lightColor;
    
    vec3 normal_unit_vector = normalize(Normal);
    vec3 light_direction_unit_vector = normalize(lightPos - FragPos);

    float diffuse_strength = max(dot(normal_unit_vector, light_direction_unit_vector), 0.0);
    vec3 diffuse_lighting = diffuse_strength * lightColor;

    float specular_strength = 0.5;
    vec3 view_direction_unit_vector = normalize(viewPos - FragPos);
    vec3 reflect_direction_unit_vector = reflect(-light_direction_unit_vector, normal_unit_vector);

    float spec = pow(max(dot(view_direction_unit_vector, reflect_direction_unit_vector),0.0), 32);
    vec3 specular_lighting = specular_strength * spec * lightColor;

    vec3 color_3 = (ambient_lighting + diffuse_lighting + specular_lighting) * objectColor;
    color = vec4(color_3, 1.0);
} 
