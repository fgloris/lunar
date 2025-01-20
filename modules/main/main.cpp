#include "window.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include <iostream>

int main() {
    auto& window = lunar::Window::getInstance();
    try {
        window.init(1800, 1200, "OpenGL");
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize window, error: " << e.what() << std::endl;
        return -1;
    }
    lunar::ShaderProgram shader_program;
    shader_program.setVertices({
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
       -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    });
    shader_program.setIndicies({
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    });
    lunar::Texture texture("../assets/wall.jpg", 0);
    texture.use();
    
    shader_program.use();
    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);
        texture.use();
        shader_program.use();
        shader_program.draw();
        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}