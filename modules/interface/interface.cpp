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
                
                if (callback_name == "empty") {
                    continue;
                }
                if (all_callbacks.find(callback_name) == all_callbacks.end()) {
                    throw std::runtime_error("Callback not found: " + callback_name);
                }
                const EventIdentifier event_identifier = convertKeyNameToEventIndetifier(key_name);
                if (registered_callbacks.find(event_identifier) != registered_callbacks.end()){
                    throw std::runtime_error("Callback already registered: " + callback_name + " for key: " + key_name);
                }

                if (binding["mod"].IsDefined()){
                    unsigned short mod_value = 0;
                    for (const auto& mod : binding["mod"]){
                        mod_value |= convertKeyNameToEventIndetifier(mod.as<std::string>());
                    }
                    registered_callbacks[event_identifier] = std::make_pair(all_callbacks[callback_name], mod_value);
                }else{
                    registered_callbacks[event_identifier] = std::make_pair(all_callbacks[callback_name], 0);
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
    if (all_callbacks.find(name) != all_callbacks.end()){
        throw std::runtime_error("Callback already registered: " + name);
    }
    all_callbacks[name] = callback;
    registered = true;
}

void Interface::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto& instance = Interface::getInstance();
    Event event = {.type = EventType::KEY, .data = {.key = {.key = (unsigned short)key, .scancode = (unsigned short)scancode, .action = (unsigned short)action, .mods = (unsigned short)mods}}};
    if (instance.registered_callbacks.find(key) != instance.registered_callbacks.end()) {
        EventCallbackFuncWithMod callback_with_mod = instance.registered_callbacks[key];
        if ((callback_with_mod.second & mods) == callback_with_mod.second){
            callback_with_mod.first(event);
        }
    }
}

void Interface::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto& instance = Interface::getInstance();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    Event event = {.type = EventType::MOUSE_CLICK, .data = {.mouse_click = {.button = (unsigned short)button, .action = (unsigned short)action, .mods = (unsigned short)mods, .xpos = (unsigned short)xpos, .ypos = (unsigned short)ypos}}};
    if (instance.registered_callbacks.find(button) != instance.registered_callbacks.end()) {
        EventCallbackFuncWithMod callback_with_mod = instance.registered_callbacks[button];
        if ((callback_with_mod.second & mods) == callback_with_mod.second){
            callback_with_mod.first(event);
        }
    }
}

void Interface::mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset){
    auto& instance = Interface::getInstance();
    Event event = {.type = EventType::MOUSE_SCROLL, .data = {.mouse_scroll = {.xoffset = (short)xoffset, .yoffset = (short)yoffset}}};
    static unsigned short scroll_ident = static_cast<EventIdentifier>(LUNAR_EVENT::LUNAR_MOUSE_SCROLL);
    unsigned short mods = getModifier(window);
    if (instance.registered_callbacks.find(scroll_ident) != instance.registered_callbacks.end()) {
        EventCallbackFuncWithMod callback_with_mod = instance.registered_callbacks[scroll_ident];
        if ((callback_with_mod.second & mods) == callback_with_mod.second){
            callback_with_mod.first(event);
        }
    }
}

void Interface::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos){
    auto& instance = Interface::getInstance();
    static unsigned short move_ident = static_cast<EventIdentifier>(LUNAR_EVENT::LUNAR_MOUSE_MOVE);
    unsigned short mouse_button_state = getMouseButtonState(window);

    Event event = {.type = EventType::MOUSE_MOVE, .data = {.mouse_move = {.mouse_button_state = mouse_button_state, .xpos = (unsigned short)xpos, .ypos = (unsigned short)ypos, .xoffset = (short)(xpos-instance.mouse_x_pos), .yoffset = (short)(ypos-instance.mouse_y_pos)}}};
    unsigned short mods = getModifier(window);
    if (instance.registered_callbacks.find(move_ident) != instance.registered_callbacks.end()) {
        EventCallbackFuncWithMod callback_with_mod = instance.registered_callbacks[move_ident];
        if ((callback_with_mod.second & mods) == callback_with_mod.second){
            callback_with_mod.first(event);
        }
    }
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

unsigned short Interface::getModifier(GLFWwindow* window){
    unsigned short mods = 0;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
        mods |= GLFW_MOD_SHIFT;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
        mods |= GLFW_MOD_CONTROL;
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
        mods |= GLFW_MOD_ALT;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS)
        mods |= GLFW_MOD_SUPER;
    return mods;
}

unsigned short Interface::getMouseButtonState(GLFWwindow* window){
    unsigned short mouse_button_state = 0;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        mouse_button_state |= 1;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        mouse_button_state   |= 2;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
        mouse_button_state |= 4;
    return mouse_button_state;
}

void Interface::debugCallback(const Event& event){
    std::cout << event.what() << std::endl;
}

Interface::~Interface() {
    registered_callbacks.clear();
    all_callbacks.clear();
}

}