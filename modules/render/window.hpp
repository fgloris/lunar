#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <functional>
#include <vector>

namespace lunar {
    struct Event;
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
        
        void init(int width, int height, const std::string& title, bool isFullscreen = false);

        [[nodiscard]] bool shouldClose() const { return glfwWindowShouldClose(window); }
        void swapBuffers() const { glfwSwapBuffers(window); }

        void pollEvents() { glfwPollEvents();}

        [[nodiscard]] GLFWwindow* getHandle() const { return window; }
        int getWidth() const { return width; }
        int getHeight() const { return height; }

        inline static bool initialized = false;
        void close(const Event& event) {glfwSetWindowShouldClose(getInstance().getHandle(), true);}
        void fullscreen(const Event& event);
        void windowed(const Event& event);
    private:
        Window() = default;
        ~Window();
        void initGLFW();
        void initWindow();
        void initGLAD();

        GLFWwindow* window{nullptr};
        int width{0}, height{0};
        std::string title;
        bool isFullscreen{false};
    };
} 