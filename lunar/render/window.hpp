#pragma once
#include <glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>

namespace lunar {
    class Window {
    public:
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

        static Window& getInstance() {
            static Window instance;
            return instance;
        }
        
        void init(int width, int height, const std::string& title);

        bool shouldClose() const { return glfwWindowShouldClose(window); }
        void swapBuffers() { glfwSwapBuffers(window); }
        void pollEvents() { glfwPollEvents(); }

        GLFWwindow* getHandle() const { return window; }
        int getWidth() const { return width; }
        int getHeight() const { return height; }

        inline static bool initialized = false;
    private:
        Window() = default;
        ~Window();
        void initGLFW();
        void initWindow();
        void initGLAD();

        GLFWwindow* window{nullptr};
        int width{0}, height{0};
        std::string title;
    };
} 