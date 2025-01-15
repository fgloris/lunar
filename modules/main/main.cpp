#include "window.hpp"
#include "shader.hpp"
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

    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);
        shader_program.draw();
        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}