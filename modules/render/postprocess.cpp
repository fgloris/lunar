#include "postprocess.hpp"
#include "shader.hpp"
#include "window.hpp"
#include <stdexcept>
#include <iostream>

namespace lunar {

static const std::string postprocess_vertex_shader = 
#include "glsllibs/postprocess-vs.glsl"
;
static std::string postprocess_fragment_shader = ShaderProgram::loadGLSLlib(
    #include "glsllibs/postprocess-fs.glsl"
    ,
    #include "glsllibs/3shade2.glsl"
);

PostProcesser::PostProcesser():shader(postprocess_vertex_shader, postprocess_fragment_shader) {
    if (!Window::initialized) {
        throw std::runtime_error("Window not initialized");
    }
    Window& w = Window::getInstance();
    
    // 创建帧缓冲
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // 创建颜色纹理
    glGenTextures(1, &colorTexture);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w.getWidth(), w.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

    // 创建深度纹理
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w.getWidth(), w.getHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    // 检查帧缓冲是否完整
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Framebuffer is not complete");
    }

    // 解绑
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    shader.setVertices<4>({
        {-1.0f,  1.0f,  0.0f, 1.0f},
        {-1.0f, -1.0f,  0.0f, 0.0f},
        { 1.0f, -1.0f,  1.0f, 0.0f},
        {-1.0f,  1.0f,  0.0f, 1.0f},
        { 1.0f, -1.0f,  1.0f, 0.0f},
        { 1.0f,  1.0f,  1.0f, 1.0f}
    });
    shader.setVertexDataProperty({"position", "TexCoords"}, {2, 2});
    shader.setIndices({0, 1, 2, 3, 4, 5});
}

void PostProcesser::tobeDrawn() {
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void PostProcesser::toDraw() {
    shader.use();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    shader.setInt("screenTexture", 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    shader.setInt("depthTexture", 1);
}

void PostProcesser::draw() {
    shader.draw();
}

}