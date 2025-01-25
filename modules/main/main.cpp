#include "render/window.hpp"
#include "render/shader.hpp"
#include "render/camera.hpp"
#include "render/texture.hpp"
#include "interface/interface.hpp"
#include "model/model.hpp"
#include <iostream>
#include <functional>
#include <cmath>

int main() {
    auto& window = lunar::Window::getInstance();
    try {
        window.init(1800, 1200, "OpenGL");
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize window, error: " << e.what() << std::endl;
        return -1;
    }

    // 创建箱子和光源的着色器程序
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
    lunar::ShaderProgram light_shader_program(light_vertex_shader_code, light_fragment_shader_code);

    // 设置顶点属性
    box_shader_program.setVertexDataProperty({"position", "normal"}, {3, 3});
    light_shader_program.setVertexDataProperty({"position"}, {3});

    // 设置立方体顶点数据
    std::vector<lunar::VertexData<3>> light_vertices = {
        {-0.5f, -0.5f, -0.5f}, 
        { 0.5f, -0.5f, -0.5f},  
        { 0.5f,  0.5f, -0.5f},  
        { 0.5f,  0.5f, -0.5f},  
        {-0.5f,  0.5f, -0.5f}, 
        {-0.5f, -0.5f, -0.5f}, 

        {-0.5f, -0.5f,  0.5f}, 
        { 0.5f, -0.5f,  0.5f},  
        { 0.5f,  0.5f,  0.5f},  
        { 0.5f,  0.5f,  0.5f},  
        {-0.5f,  0.5f,  0.5f}, 
        {-0.5f, -0.5f,  0.5f}, 

        {-0.5f,  0.5f,  0.5f}, 
        {-0.5f,  0.5f, -0.5f}, 
        {-0.5f, -0.5f, -0.5f}, 
        {-0.5f, -0.5f, -0.5f}, 
        {-0.5f, -0.5f,  0.5f}, 
        {-0.5f,  0.5f,  0.5f}, 

        { 0.5f,  0.5f,  0.5f},  
        { 0.5f,  0.5f, -0.5f},  
        { 0.5f, -0.5f, -0.5f},  
        { 0.5f, -0.5f, -0.5f},  
        { 0.5f, -0.5f,  0.5f},  
        { 0.5f,  0.5f,  0.5f},  

        {-0.5f, -0.5f, -0.5f}, 
        { 0.5f, -0.5f, -0.5f},  
        { 0.5f, -0.5f,  0.5f},  
        { 0.5f, -0.5f,  0.5f},  
        {-0.5f, -0.5f,  0.5f}, 
        {-0.5f, -0.5f, -0.5f}, 

        {-0.5f,  0.5f, -0.5f}, 
        { 0.5f,  0.5f, -0.5f},  
        { 0.5f,  0.5f,  0.5f},  
        { 0.5f,  0.5f,  0.5f},  
        {-0.5f,  0.5f,  0.5f}, 
        {-0.5f,  0.5f, -0.5f}, 
    };

    std::vector<lunar::VertexData<6>> box_vertices = {
        {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f},
        { 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f}, 
        { 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f}, 
        { 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f}, 
        {-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f}, 
        {-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f}, 
        {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f},
        { 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f},
        { 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f},
        { 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f},
        {-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f},
        {-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f},
        {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f},
        {-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
        {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
        {-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f},
        {-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f},
        {-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f},
        { 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f},
        { 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
        { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
        { 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f},
        { 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f},
        { 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f},
        {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},
        { 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},
        { 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
        { 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
        {-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f},
        {-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f},
        {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f},
        { 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f},
        { 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
        { 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
        {-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f},
        {-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f}
    };


    box_shader_program.setVertices<6>(box_vertices);
    light_shader_program.setVertices<3>(light_vertices);

    box_shader_program.setSequentialIndices();
    light_shader_program.setSequentialIndices();

    lunar::Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));

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

    glm::mat4 box_model = glm::mat4(1.0f);
    box_model = glm::rotate(box_model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat3 box_normal_matrix = lunar::Model::getNormalMatrix(box_model);

    glEnable(GL_DEPTH_TEST);
    while (!window.shouldClose()) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 计算光源位置
        float time = static_cast<float>(glfwGetTime());
        glm::vec3 lightPos(
            2.0f * cos(time),  // x坐标
            1.0f,             // y坐标保持不变
            2.0f * sin(time)  // z坐标
        );

        // 更新光源模型矩阵
        glm::mat4 light_model = glm::mat4(1.0f);
        light_model = glm::translate(light_model, lightPos);
        light_model = glm::scale(light_model, glm::vec3(0.2f));

        // 渲染箱子
        box_shader_program.use();
        box_shader_program.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        box_shader_program.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        box_shader_program.setVec3("lightPos", lightPos);  // 使用更新后的光源位置
        box_shader_program.setVec3("viewPos", camera.getPosition());

        glm::mat4 view = camera.computeViewMatrix();
        glm::mat4 projection = camera.computeProjectionMatrix();
        
        box_shader_program.setMat4("model", box_model);
        box_shader_program.setMat4("view", view);
        box_shader_program.setMat4("projection", projection);
        box_shader_program.setMat3("normalMatrix", box_normal_matrix);
        box_shader_program.draw();

        // 渲染光源立方体
        light_shader_program.use();
        
        light_shader_program.setMat4("model", light_model);
        light_shader_program.setMat4("view", view);
        light_shader_program.setMat4("projection", projection);
        light_shader_program.draw();

        window.swapBuffers();
        window.pollEvents();
    }

    return 0;
}