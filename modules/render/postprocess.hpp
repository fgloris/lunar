#pragma once
#include "shader.hpp"
namespace lunar {
static const std::string postprocess_vertex_shader = 
#include "glsllibs/postprocess-vs.glsl"
;
static const std::string postprocess_fragment_shader = 
#include "glsllibs/postprocess-fs.glsl"
;
class PostProcesser {
public:
    PostProcesser();
    void tobeDrawn();
    void toDraw();
    void draw();
private:
    ShaderProgram shader;
    unsigned int framebuffer, texture, renderbuffer;
};

}



