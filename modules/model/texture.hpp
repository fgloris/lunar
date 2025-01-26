#pragma once
#include "stb_image/stb_image.h"
#include <glad/glad.h>
#include <string>

namespace lunar{

enum class TextureType{
    Specular,
    Diffuse
};

class Texture{
public:
    Texture(const std::string &filename,
    const unsigned int id,
    const unsigned int expand_param = GL_MIRRORED_REPEAT,
    const unsigned int filter_param_max = GL_LINEAR,
    const unsigned int filter_param_min = GL_NEAREST,
    bool generate_mitmap = true,
    bool flip_y = false
    );
    ~Texture();
    void use() const;
    unsigned int getID() const {return id;}
    std::string name;
private:
    unsigned int texture, id;
};

}