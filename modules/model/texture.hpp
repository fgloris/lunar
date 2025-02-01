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

    Texture(const std::string &name,
        TextureType type,
        const unsigned int expand_param,
        const unsigned int filter_param_max,
        const unsigned int filter_param_min,
        bool generate_mitmap,
        bool flip_y,
        unsigned char* data,
        int width,
        int height,
        int channels
    );

    unsigned int id;
    std::string path;
    TextureType type;
    bool operator==(const std::string& other) const {
        return path == other;
    }
};

class TextureCollector{
public:
    static TextureCollector& getInstance(){
        static TextureCollector instance;
        return instance;
    }
    void registerTexture(unsigned int id);
private:
    ~TextureCollector();
    TextureCollector() = default;
    TextureCollector(const TextureCollector&) = delete;
    TextureCollector& operator=(const TextureCollector&) = delete;
    std::vector<unsigned int> texture_ids;
};
}