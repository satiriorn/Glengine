#version 330 core
out vec4 FragColor;

struct Light {
    //vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in    vec3 FragPos;
in    vec2 TexCoords;
in    vec3 TangentLightPos;
in    vec3 TangentViewPos;
in    vec3 TangentFragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform float shininess;

void main()
{    
	// obtain normal from normal map in range [0,1]
    vec3 normal = texture(texture_normal1, TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
	
	// get diffuse color
    vec3 color = texture(texture_diffuse1, TexCoords).rgb;
    // ambient
    vec3 ambient = 0.2 * color;
    // diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = vec3(0.2) * spec * texture(texture_specular1, TexCoords).rgb;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
	//FragColor =vec4(texture(texture_diffuse1, TexCoords).rgb,1.0);
}
