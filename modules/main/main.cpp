#include "render/window.hpp"
#include "render/shader.hpp"
#include "render/camera.hpp"
#include "render/texture.hpp"
#include "interface/interface.hpp"
#include <iostream>
#include <functional>

int main() {
    auto& window = lunar::Window::getInstance();
    try {
        window.init(1800, 1200, "OpenGL");
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize window, error: " << e.what() << std::endl;
        return -1;
    }
    const std::string box_vertex_shader_code = 
    #include "../modules/render/GLSL/phong-shading-box-vs.glsl"
    ;
    const std::string box_fragment_shader_code = 
    #include "../modules/render/GLSL/phong-shading-box-fs.glsl"
    ;
    const std::string light_vertex_shader_code = 
    #include "../modules/render/GLSL/phong-shading-light-vs.glsl"
    ;
    const std::string light_fragment_shader_code = 
    #include "../modules/render/GLSL/phong-shading-light-fs.glsl"
    ;
    lunar::ShaderProgram box_shader_program(box_vertex_shader_code, box_fragment_shader_code);
    box_shader_program.setVertexDataProperty({"position", "color"},{3,3});
    box_shader_program.setVertices<6>({
        {-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f},
        {-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f},
        {-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f},

        {-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f},
        {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f},
        {-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f},

        {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f},
        {-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f},
        {-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f},
        {-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f},
        {-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f},
        {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f},

        { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f},

        {-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f},
        {-0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f},
        {-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f},

        {-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f},
        { 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f},
        {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f},
        {-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f},
        {-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f}
    });
    box_shader_program.setSequentialIndices();

    lunar::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    lunar::Interface& interface = lunar::Interface::getInstance();

    interface.registerCallback("window_close", std::bind(&lunar::Window::close, &window, std::placeholders::_1));
    interface.registerCallback("window_fullscreen", std::bind(&lunar::Window::fullscreen, &window, std::placeholders::_1));
    interface.registerCallback("window_windowed", std::bind(&lunar::Window::windowed, &window, std::placeholders::_1));
    interface.registerCallback("camera_move_forward", std::bind(&lunar::Camera::MoveForward, &camera, std::placeholders::_1));
    interface.registerCallback("camera_move_backward", std::bind(&lunar::Camera::MoveBackward, &camera, std::placeholders::_1));
    interface.registerCallback("camera_move_left", std::bind(&lunar::Camera::MoveLeft, &camera, std::placeholders::_1));
    interface.registerCallback("camera_move_right", std::bind(&lunar::Camera::MoveRight, &camera, std::placeholders::_1));
    interface.registerCallback("camera_rotate", std::bind(&lunar::Camera::Rotate, &camera, std::placeholders::_1));
    interface.registerCallback("camera_reset_zoom", std::bind(&lunar::Camera::resetZoom, &camera));

    interface.registerCallback("camera_zoom", std::bind(&lunar::Camera::Zoom, &camera, std::placeholders::_1));
    interface.bindAllCallbacks("../modules/config/interface.yaml", window.getHandle());

    glEnable(GL_DEPTH_TEST);
    while (!window.shouldClose()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.5f, 1.0f, 0.0f));

        box_shader_program.use();
        box_shader_program.setTransform(camera.computeProjectionMatrix() * camera.computeViewMatrix() * model);
        box_shader_program.draw();
        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}