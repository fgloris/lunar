#pragma once
#include <map>
#include <string>
#include <functional>
#include "GLFW/glfw3.h"
#include "fmt/format.h"

namespace lunar {
enum class EventType {
    KEY,
    MOUSE_CLICK,
    MOUSE_MOVE,
    MOUSE_SCROLL,
};

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
        } mouse_move;
        struct {
            double x_offset;
            double y_offset;
        } mouse_scroll;
    } data;
    [[nodiscard]] std::string what() const{
        switch (type)
        {
        case EventType::KEY:
            return fmt::format("<keyboard: key:{}, action:{}, scancode:{}, mods:{}>",
                                    data.key.key,data.key.action,data.key.scancode,data.key.mods);
        case EventType::MOUSE_CLICK:
            return fmt::format("<mouse click: button:{}, x:{}, y:{}>",data.mouse_click.button,data.mouse_click.xpos,data.mouse_click.ypos);
        case EventType::MOUSE_MOVE:
            return fmt::format("<mouse move: x:{}, y:{}>",data.mouse_move.xpos,data.mouse_move.ypos);
        default:
            return "<unknown event>";
        }
    }
};

class Interface {
public:
    explicit Interface(const std::string& path);
    ~Interface();
    std::map<std::string, std::function<void(Event)>> callbacks;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);

private:
    int convertKeyNameToGLFW(const std::string& key_name);
};
}