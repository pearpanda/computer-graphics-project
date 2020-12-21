#version 460 core

#define MAX_DIRECTIONAL_LIGHTS 2
#define MAX_POINT_LIGHTS 4
#define MAX_SPOTLIGHTS 4

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct LightColor {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Attenuation {
    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight {
    vec3 direction;
    LightColor color;
};

struct PointLight {
    vec3 position;
    LightColor color;
    Attenuation attenuation;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    // The angles are stored as cosines
    float cutoff_angle;
    float weaken_angle;

    LightColor color;
    Attenuation attenuation;
};

struct Observer {
    vec3 position;
    vec3 direction;
};

out vec4 FragColor;

// Input data from vertex shader
// -----------------------------
in vec3 position;
in vec3 normal;
in vec2 tex_coords;

// Lights
// ------
uniform DirectionalLight directional_lights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform SpotLight spotlights[MAX_SPOTLIGHTS];
uniform int active_directional_lights;
uniform int active_point_lights;
uniform int active_spotlights;

uniform Material material;
uniform Observer camera;

float attenuation(Attenuation attenuation, float distance) {
    return 1.0f / (attenuation.constant + attenuation.linear * distance +
                   attenuation.quadratic * distance * distance);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 norm,
                               vec3 view_direction) {
    vec3 ambient_color = vec3(texture(material.texture_diffuse1, tex_coords));
    vec3 diffuse_color = vec3(texture(material.texture_diffuse1, tex_coords));
    vec3 specular_color = vec3(texture(material.texture_specular1, tex_coords));

    vec3 light_dir = normalize(-light.direction);

    // Ambient
    // -------
    vec3 ambient = light.color.ambient * ambient_color;

    // Diffuse
    // -------
    float diff = max(dot(norm, light_dir), 0.0f);
    vec3 diffuse = light.color.diffuse * diff * diffuse_color;

    // Specular
    // --------
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(reflect_dir, view_direction), 0.0f),
                     material.shininess);
    vec3 specular = light.color.specular * spec * specular_color;

    return (ambient + diffuse + specular);
}

vec3 calculatePointLight(PointLight light, vec3 norm, vec3 view_direction) {
    vec3 ambient_color = vec3(texture(material.texture_diffuse1, tex_coords));
    vec3 diffuse_color = vec3(texture(material.texture_diffuse1, tex_coords));
    vec3 specular_color = vec3(texture(material.texture_specular1, tex_coords));

    vec3 light_dir = normalize(light.position - position);

    // Ambient
    // -------
    vec3 ambient = light.color.ambient * ambient_color;

    // Diffuse
    // -------
    float diff = max(dot(norm, light_dir), 0.0f);
    vec3 diffuse = light.color.diffuse * diff * diffuse_color;

    // Specular
    // --------
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(reflect_dir, view_direction), 0.0f),
                     material.shininess);
    vec3 specular = light.color.specular * spec * specular_color;

    float distance = length(light_dir);
    float attenuation = attenuation(light.attenuation, distance);
    return attenuation * (ambient + diffuse + specular);
}

vec3 calculateSpotLight(SpotLight light, vec3 norm, vec3 view_direction) {
    vec3 ambient_color = vec3(texture(material.texture_diffuse1, tex_coords));
    vec3 diffuse_color = vec3(texture(material.texture_diffuse1, tex_coords));
    vec3 specular_color = vec3(texture(material.texture_specular1, tex_coords));

    vec3 light_dir = normalize(light.position - position);

    // Ambient
    // -------
    vec3 ambient = light.color.ambient * ambient_color;

    // Diffuse
    // -------
    float diff = max(dot(norm, light_dir), 0.0f);
    vec3 diffuse = light.color.diffuse * diff * diffuse_color;

    // Specular
    // --------
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(reflect_dir, view_direction), 0.0f),
                     material.shininess);
    vec3 specular = light.color.specular * spec * specular_color;

    float distance = length(light_dir);
    float attenuation = attenuation(light.attenuation, distance);

    float theta = dot(light_dir, normalize(-light.direction));
    float epsilon = light.weaken_angle - light.cutoff_angle;
    float intensity = clamp((theta - light.cutoff_angle) / epsilon, 0.0f, 1.0f);

    return intensity * attenuation * (ambient + diffuse + specular);
}

void main() {
    vec3 norm = normalize(normal);
    vec3 view_direction = normalize(camera.position - position);

    vec3 color = vec3(0.0f);
    for (int i = 0; i < active_directional_lights; ++i)
        color += calculateDirectionalLight(directional_lights[i], norm,
                                           view_direction);
    for (int i = 0; i < active_point_lights; ++i)
        color += calculatePointLight(point_lights[i], norm, view_direction);
    for (int i = 0; i < active_spotlights; ++i)
        color += calculateSpotLight(spotlights[i], norm, view_direction);

    FragColor = vec4(color, 1.0f);
}
