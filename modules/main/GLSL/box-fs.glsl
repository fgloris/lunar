R"(
#version 430 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

vec3 rgbToHsv(vec3 rgb) {
    float max = max(rgb.r, max(rgb.g, rgb.b));
    float min = min(rgb.r, min(rgb.g, rgb.b));
    float delta = max - min;
    float hue = 0.0;
    if (delta != 0.0) {
        if (max == rgb.r) hue = (rgb.g - rgb.b) / delta;
        else if (max == rgb.g) hue = (rgb.b - rgb.r) / delta + 2.0;
        else hue = (rgb.r - rgb.g) / delta + 4.0;
    }
    return vec3(hue, delta / max, max);
}

vec3 hsvToRgb(vec3 hsv) {
    float h = hsv.x;
    float s = hsv.y;
    float v = hsv.z;
    float c = v * s;
    float x = c * (1.0 - abs(mod(h * 6.0, 2.0) - 1.0));
    float m = v - c;
    return vec3(m + c, m + x, m);
}

vec3 cartoonify(vec3 rgb) {
    vec3 hsv = rgbToHsv(rgb);
    float value = hsv.z;
    value = float(int(value * 5)) / 5;
    return hsvToRgb(vec3(hsv.x, hsv.y, value));
}

void main()
{
    // ambient
    vec3 ambient = light.ambient * material.ambient;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.color * diff * light.diffuse * material.diffuse;

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.color * spec * light.specular * material.specular;

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}
)"