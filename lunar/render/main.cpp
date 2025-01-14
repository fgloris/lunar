#include "window.hpp"
#include <iostream>

int main() {
    auto& window = lunar::Window::getInstance();
    try {
        window.init(1800, 1600, "OpenGL");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT);
        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}