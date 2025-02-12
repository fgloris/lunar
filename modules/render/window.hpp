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
        void init(std::string config_path);

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
        int default_width{2560}, default_height{1600};
        int width{default_width}, height{default_height};
        std::string title;
        bool isFullscreen{false};
    };
} 