R"(
#version 430 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
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
in vec2 TexCoords;

out vec4 fragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

// 添加三渲二相关的函数
vec3 RGB2HSV(vec3 rgb) {
    float max = max(rgb.r, max(rgb.g, rgb.b));
    float min = min(rgb.r, min(rgb.g, rgb.b));
    float delta = max - min;
    
    vec3 hsv = vec3(0.0, 0.0, max);  // v = max
    
    if (max != 0.0) {  // s = delta/max
        hsv.y = delta / max;
    }
    
    if (delta != 0.0) {
        if (max == rgb.r) {
            hsv.x = (rgb.g - rgb.b) / delta;  // h
            if (rgb.g < rgb.b) 
                hsv.x += 6.0;
        } else if (max == rgb.g) {
            hsv.x = 2.0 + (rgb.b - rgb.r) / delta;
        } else {
            hsv.x = 4.0 + (rgb.r - rgb.g) / delta;
        }
        hsv.x /= 6.0;
    }
    
    return hsv;
}

vec3 HSV2RGB(vec3 hsv) {
    if (hsv.y == 0.0) {
        return vec3(hsv.z);
    }
    
    float h = hsv.x * 6.0;
    float i = floor(h);
    float f = h - i;
    float p = hsv.z * (1.0 - hsv.y);
    float q = hsv.z * (1.0 - hsv.y * f);
    float t = hsv.z * (1.0 - hsv.y * (1.0 - f));
    
    if (i == 0.0) return vec3(hsv.z, t, p);
    else if (i == 1.0) return vec3(q, hsv.z, p);
    else if (i == 2.0) return vec3(p, hsv.z, t);
    else if (i == 3.0) return vec3(p, q, hsv.z);
    else if (i == 4.0) return vec3(t, p, hsv.z);
    else return vec3(hsv.z, p, q);
}

vec3 color_thinning(vec3 rgb, float threshold) {
    vec3 hsv = RGB2HSV(rgb);
    float value = hsv.z;
    value = float(int((value * threshold) + 0.5)) / threshold;
    return HSV2RGB(vec3(hsv.x, hsv.y, value));
}

float getEdgeStrength(vec3 normal, vec3 viewDir) {
    float edge = 1.0 - max(dot(normalize(normal), viewDir), 0.0);
    if (edge > 0.8) return 1.0;
    else return 0.0;
}

void main()
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 viewDir = normalize(viewPos - fragPos);

    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.color * diff * light.diffuse * vec3(texture(material.diffuse, TexCoords));

    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.color * spec * light.specular * vec3(texture(material.specular, TexCoords));

    // 基础光照结果
    vec3 result = ambient + diffuse + specular;
    
    // 应用三渲二效果
    result = color_thinning(result, 2.0);
    
    // 添加边缘
    float edgeStrength = getEdgeStrength(normal, viewDir);
    vec3 edgeColor = vec3(0.0); // 黑色边缘
    result = mix(result, edgeColor, edgeStrength);

    fragColor = vec4(result, 1.0);
}
)"