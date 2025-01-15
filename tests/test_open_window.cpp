#include "window.hpp"
#include "shader.hpp"
#include <gtest/gtest.h>
#include <memory>

class WindowTest : public ::testing::Test {
protected:
    void SetUp() override {
        window = &lunar::Window::getInstance();
        window->init(800, 600, "OpenGL Test");
    }

    void TearDown() override {
        // Window是单例，会自动清理
    }

    lunar::Window* window;
};

TEST_F(WindowTest, WindowInitialization) {
    EXPECT_NE(window->getHandle(), nullptr);
    EXPECT_EQ(window->getWidth(), 800);
    EXPECT_EQ(window->getHeight(), 600);
}

TEST_F(WindowTest, ShaderCompilation) {
    // 测试着色器编译
    EXPECT_NO_THROW({
        lunar::Shader vertex_shader(GL_VERTEX_SHADER);
        lunar::Shader fragment_shader(GL_FRAGMENT_SHADER);
    });
}

TEST_F(WindowTest, ShaderProgramLinking) {
    lunar::Shader vertex_shader(GL_VERTEX_SHADER);
    lunar::Shader fragment_shader(GL_FRAGMENT_SHADER);
    
    EXPECT_NO_THROW({
        lunar::ShaderProgram shader_program(vertex_shader, fragment_shader);
    });
}

TEST_F(WindowTest, AutoConstruct){
    EXPECT_NO_THROW({
        lunar::ShaderProgram shader_program;
    });
}

TEST_F(WindowTest, BasicRendering) {
    lunar::Shader vertex_shader(GL_VERTEX_SHADER);
    lunar::Shader fragment_shader(GL_FRAGMENT_SHADER);
    lunar::ShaderProgram shader_program(vertex_shader, fragment_shader);
    shader_program.setVertices({
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // 左下，红色
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // 右下，绿色
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // 顶部，蓝色
    });

    // 测试一帧渲染
    EXPECT_NO_THROW({
        glClear(GL_COLOR_BUFFER_BIT);
        shader_program.draw();
        window->swapBuffers();
        window->pollEvents();
    });
}