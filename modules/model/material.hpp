#pragma once

#include <glm/glm.hpp>
#include <map>
#include <string>

namespace lunar {

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct StrongPointLight {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct ParallelLight {
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};


struct MaterialTexture {
    unsigned int diffuse;    // 漫反射贴图的纹理单元
    unsigned int specular;   // 镜面光贴图的纹理单元
    float shininess;
};

class General {
    public:
    static std::pair<double, double> getPointAttenuationFactor(float distance);
    static glm::mat3 getNormalMatrix(glm::mat4 model);
    static std::map<std::string, Material> materials;
    static std::map<float, std::pair<double, double>> point_attenuation_factors;
};
}