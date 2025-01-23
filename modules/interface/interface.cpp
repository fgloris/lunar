#include "interface.hpp"
#include "fmt/format.h"
#include <yaml-cpp/yaml.h>
#include <iostream>

namespace lunar {
[[nodiscard]] std::string Event::what() const{
    switch (type)
    {
    case EventType::KEY:
        return fmt::format("<keyboard: key:{}, action:{}, scancode:{}, mods:{}>",
                                data.key.key,data.key.action,data.key.scancode,data.key.mods);
    case EventType::MOUSE_CLICK:
        return fmt::format("<mouse click: button:{}, x:{}, y:{}>",
                                data.mouse_click.button,data.mouse_click.xpos,data.mouse_click.ypos);
    case EventType::MOUSE_MOVE:
        return fmt::format("<mouse move: x:{}, y:{}, xoffset:{}, yoffset:{}>",
                                data.mouse_move.xpos,data.mouse_move.ypos,data.mouse_move.xoffset,data.mouse_move.yoffset);
    case EventType::MOUSE_SCROLL:
        return fmt::format("<mouse scroll: x:{}, y:{}>",
                                data.mouse_scroll.xoffset,data.mouse_scroll.yoffset);
    default: return "<unknown event>";
    }
}

void Interface::bindAllCallbacks(const std::string& config_path, GLFWwindow* window) {
    try {
        YAML::Node config = YAML::LoadFile(config_path);
        
        if (config["keyboard_and_mouse_bindings"]) {
            for (const auto& binding : config["keyboard_and_mouse_bindings"]) {
                std::string key_name = binding["key"].as<std::string>();
                std::string callback_name = binding["callback"].as<std::string>();
                
                unsigned short mod = 0;
                if (binding["mod"].IsDefined()){
                    mod = convertKeyNameToEventIndetifier(binding["mod"].as<std::string>());
                }

                if (callback_name == "empty") {
                    continue;
                }

                const EventIdentifier event_identifier = convertKeyNameToEventIndetifier(key_name);
                
                if (all_callbacks.find(callback_name) != all_callbacks.end()) {
                    registered_callbacks[event_identifier] = all_callbacks[callback_name];
                } else {
                    throw std::runtime_error("Callback not found: " + callback_name);
                }
            }
        }
        
        if (config["keyboard_and_mouse_settings"]){
            YAML::Node settings = config["keyboard_and_mouse_settings"];
            reset_mouse_position_upon_enter_window = settings["reset_mouse_position_upon_enter_window"].as<bool>(); 
        }
    } catch (const YAML::Exception& e) {
        std::cerr << "Error loading control config: " << e.what() << std::endl;
    }
    glfwGetCursorPos(window, &mouse_x_pos, &mouse_y_pos);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    glfwSetCursorPosCallback(window, mouseMoveCallback);
    glfwSetCursorEnterCallback(window, mouseEnterCallback);
    bound = true;
}

void Interface::registerCallback(const std::string& name, EventCallbackFunction callback) {
    all_callbacks[name] = callback;
}

void Interface::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto& registered_callbacks = Interface::getInstance().registered_callbacks;
    Event event = {.type = EventType::KEY, .data = {.key = {key, scancode, action, mods}}};
    registered_callbacks[key](event);
}

void Interface::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto& registered_callbacks = Interface::getInstance().registered_callbacks;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    Event event = {.type = EventType::MOUSE_CLICK, .data = {.mouse_click = {button, action, mods, xpos, ypos}}};
    registered_callbacks[button](event);
}

void Interface::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset){
    auto& registered_callbacks = Interface::getInstance().registered_callbacks;
    Event event = {.type = EventType::MOUSE_SCROLL, .data = {.mouse_scroll = {xoffset, yoffset}}};
    registered_callbacks[static_cast<EventIdentifier>(LUNAR_EVENT::LUNAR_MOUSE_SCROLL)](event);
}

void Interface::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos){
    auto& instance = Interface::getInstance();
    auto& registered_callbacks = instance.registered_callbacks;
    Event event = {.type = EventType::MOUSE_MOVE, .data = {.mouse_move = {.xpos = xpos, .ypos = ypos, .xoffset = xpos-instance.mouse_x_pos, .yoffset = ypos-instance.mouse_y_pos}}};
    registered_callbacks[static_cast<EventIdentifier>(LUNAR_EVENT::LUNAR_MOUSE_MOVE)](event);
    instance.mouse_x_pos = xpos;
    instance.mouse_y_pos = ypos;
}

void Interface::mouseEnterCallback(GLFWwindow* window, int entered){
    auto& instance = Interface::getInstance();
    if (entered){
        if (instance.reset_mouse_position_upon_enter_window){
            glfwGetCursorPos(window, &instance.mouse_x_pos, &instance.mouse_y_pos);
        }
    }
}

void Interface::debugCallback(const Event& event){
    std::cout << event.what() << std::endl;
}

Interface::~Interface() {
    registered_callbacks.clear();
    all_callbacks.clear();
}

}