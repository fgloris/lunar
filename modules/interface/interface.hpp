#pragma once
#include <map>
#include <string>
#include <functional>
#include "GLFW/glfw3.h"

namespace lunar {
enum class EventType {
    KEY,
    MOUSE_CLICK,
    MOUSE_MOVE,
    MOUSE_SCROLL,
    EMPTY,
};

using EventIdentifier = unsigned short;

struct Event {
    EventType type;
    union {
        struct {
            int key;
            int action;
            int scancode;
            int mods;
        } key;
        struct {
            int button;
            double xpos;
            double ypos;
        } mouse_click;
        struct {
            double xpos;
            double ypos;
        } mouse_move_or_scroll;
    } data;
    [[nodiscard]] std::string what() const;
};

class Interface {
public:
    ~Interface();
    std::map<std::string, std::function<void(Event)>> all_callbacks;
    std::map<EventIdentifier, std::function<void(Event)>> registered_callbacks;

    static Interface& getInstance() {
        static Interface instance;
        return instance;
    }
    void bindCallbacks(const std::string& path, GLFWwindow* window);
    void registerCallback(const std::string& name, std::function<void(Event)> callback);

    inline static bool bound = false;
    inline static bool registered = false;
private:
    Interface() = default;
    static EventIdentifier convertKeyNameToEventIndetifier(const std::string& key_name);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
};
}
