#pragma once
#include <map>
#include <string>
#include <functional>
#include "GLFW/glfw3.h"

namespace lunar {
enum class EventType {
    KEY = 0,
    MOUSE_CLICK = 1,
    MOUSE_MOVE = 2,
    MOUSE_SCROLL = 3,
};

enum class LUNAR_EVENT{
    LUNAR_MOUSE_SCROLL = 1000,
    LUNAR_MOUSE_MOVE = 1001,
};

struct Event {
    EventType type;
    union {
        struct {
            int key;
            int scancode;
            int action;
            int mods;
        } key;
        struct {
            int button;
            int action;
            int mods;
            double xpos;
            double ypos;
        } mouse_click;
        struct {
            //int button;
            //int mods;
            double xpos;
            double ypos;
            double xoffset;
            double yoffset;
        } mouse_move;
        struct {
            //int mods;
            double xoffset;
            double yoffset;
        } mouse_scroll;
    } data;
    [[nodiscard]] std::string what() const;
};

using EventIdentifier = unsigned short;
using EventCallbackFunction = std::function<void(const Event&)>;
using EventCallbackFuncWithMod = std::pair<EventCallbackFunction, unsigned short>;

class Interface {
public:
    ~Interface();
    std::map<std::string, EventCallbackFunction> all_callbacks;
    std::map<EventIdentifier, EventCallbackFunction> registered_callbacks;
    std::map<EventIdentifier, EventCallbackFuncWithMod> registered_callbacks_with_mod;

    static Interface& getInstance() {
        static Interface instance;
        return instance;
    }
    void bindAllCallbacks(const std::string& path, GLFWwindow* window);
    void registerCallback(const std::string& name, EventCallbackFunction callback);

    inline static bool bound = false;
    inline static bool registered = false;
private:
    Interface() = default;
    static EventIdentifier convertKeyNameToEventIndetifier(const std::string& key_name);
    static void debugCallback(const Event& event);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseEnterCallback(GLFWwindow* window, int entered);
    static unsigned short getModifier(GLFWwindow* window);

    double mouse_x_pos, mouse_y_pos;
    bool reset_mouse_position_upon_enter_window = false;
};
}