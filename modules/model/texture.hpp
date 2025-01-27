#pragma once
#include "stb_image/stb_image.h"
#include <glad/glad.h>
#include <string>
#include <vector>

namespace lunar{

enum class TextureType{
    Specular,
    Diffuse
};

struct Texture{
    Texture(const std::string &filename,
    TextureType type = TextureType::Diffuse,
    const unsigned int expand_param = GL_MIRRORED_REPEAT,
    const unsigned int filter_param_max = GL_LINEAR,
    const unsigned int filter_param_min = GL_NEAREST,
    bool generate_mitmap = true,
    bool flip_y = false
    );
    ~Texture();
    unsigned int id;
    std::string path;
    TextureType type;
};
}