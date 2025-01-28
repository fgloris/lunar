#include "camera.hpp"
#include "window.hpp"
#include "interface/interface.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace lunar{
    Camera::Camera(glm::vec3 camera_pos, float zoom):zoom(zoom),
    camera_pos(camera_pos), camera_direction(glm::vec3(0.0f, 0.0f, 1.0f)){
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        camera_right = glm::normalize(glm::cross(up, camera_direction));
        camera_up = glm::normalize(glm::cross(camera_direction, camera_right));
    }
    glm::mat4 Camera::computeViewMatrix() const {
        return glm::lookAt(camera_pos, camera_pos - camera_direction, camera_up);
    }

    glm::mat4 Camera::computeProjectionMatrix() const{
        if (!Window::initialized){
            throw std::runtime_error("Window not initialized");
        }else {
            Window& w = Window::getInstance();
            return glm::perspective(glm::radians(static_cast<float>(zoom)), static_cast<float>(w.getWidth()) / static_cast<float>(w.getHeight()), 0.1f, 100.0f);
        }
    }

    void Camera::Zoom(const Event& event){
        zoom -= event.data.mouse_scroll.yoffset * zoom_speed;
    }

    void Camera::MoveForward(const Event& event){
        switch (event.type){
            case EventType::MOUSE_MOVE:
            case EventType::MOUSE_SCROLL:
                camera_pos -= static_cast<float>(event.data.mouse_scroll.yoffset * move_speed) * camera_direction;
                break;
            case EventType::KEY:
                if (event.data.key.action == GLFW_PRESS || event.data.key.action == GLFW_REPEAT){
                    camera_pos -= 0.1f * move_speed * camera_direction;
                }
                break;
            case EventType::MOUSE_CLICK:
                camera_pos -= 0.1f * move_speed * camera_direction;
                break;
            default:
                break;
        }
    }
    void Camera::MoveBackward(const Event& event){
        switch (event.type){
            case EventType::MOUSE_MOVE:
            case EventType::MOUSE_SCROLL:
                camera_pos += (float)(event.data.mouse_scroll.yoffset * move_speed) * camera_direction;
                break;
            case EventType::KEY:
                if (event.data.key.action == GLFW_PRESS || event.data.key.action == GLFW_REPEAT){
                    camera_pos += 0.1f * move_speed * camera_direction;
                }
                break;
            case EventType::MOUSE_CLICK:
                if (event.data.mouse_click.action == GLFW_PRESS){
                    camera_pos += 0.1f * move_speed * camera_direction;
                }
                break;
            default:
                break;
        }
    }
    void Camera::MoveLeft(const Event& event){
        switch (event.type){
            case EventType::MOUSE_MOVE:
            case EventType::MOUSE_SCROLL:
                camera_pos += (float)(event.data.mouse_scroll.xoffset * move_speed) * camera_right;
                break;
            case EventType::KEY:
                if (event.data.key.action == GLFW_PRESS || event.data.key.action == GLFW_REPEAT){
                    camera_pos += 0.1f * move_speed * camera_right;
                }
                break;
            case EventType::MOUSE_CLICK:
                if (event.data.mouse_click.action == GLFW_PRESS){
                    camera_pos += 0.1f * move_speed * camera_right;
                }
                break;
            default:
                break;
        }
    }

    void Camera::MoveRight(const Event& event){
        switch (event.type){
            case EventType::MOUSE_MOVE:
            case EventType::MOUSE_SCROLL:
                camera_pos -= static_cast<float>(event.data.mouse_scroll.xoffset * move_speed) * camera_right;
                break;
            case EventType::KEY:
                if (event.data.key.action == GLFW_PRESS || event.data.key.action == GLFW_REPEAT){
                    camera_pos -= 0.1f * move_speed * camera_right;
                }
                break;
            case EventType::MOUSE_CLICK:
                if (event.data.mouse_click.action == GLFW_PRESS){
                    camera_pos -= 0.1f * move_speed * camera_right;
                }
                break;
            default:
                break;
        }
    }

    void Camera::MoveUp(const Event& event){
        camera_pos += 0.1f * move_speed * camera_up;
    }

    void Camera::MoveDown(const Event& event){
        camera_pos -= 0.1f * move_speed * camera_up;
    }

    void Camera::registerCallback(Interface& interface){
        interface.registerCallback("camera_move_forward", std::bind(&lunar::Camera::MoveForward, this, std::placeholders::_1));
        interface.registerCallback("camera_move_backward", std::bind(&lunar::Camera::MoveBackward, this, std::placeholders::_1));
        interface.registerCallback("camera_move_left", std::bind(&lunar::Camera::MoveLeft, this, std::placeholders::_1));
        interface.registerCallback("camera_move_right", std::bind(&lunar::Camera::MoveRight, this, std::placeholders::_1));
        interface.registerCallback("camera_rotate", std::bind(&lunar::Camera::Rotate, this, std::placeholders::_1));
        interface.registerCallback("camera_reset_zoom", std::bind(&lunar::Camera::resetZoom, this));
        interface.registerCallback("camera_move_up", std::bind(&lunar::Camera::MoveUp, this, std::placeholders::_1));
        interface.registerCallback("camera_move_down", std::bind(&lunar::Camera::MoveDown, this, std::placeholders::_1));
        interface.registerCallback("camera_zoom", std::bind(&lunar::Camera::Zoom, this, std::placeholders::_1));
    }

    void Camera::Rotate(const Event& event){
        camera_direction = glm::normalize(glm::rotate(camera_direction, static_cast<float>(event.data.mouse_move.xoffset * rotate_speed), camera_up));
        camera_direction = glm::normalize(glm::rotate(camera_direction, static_cast<float>(event.data.mouse_move.yoffset * rotate_speed), camera_right));
        camera_right = glm::normalize(glm::cross(camera_up, camera_direction));
        camera_up = glm::normalize(glm::cross(camera_direction, camera_right));
    }
    
}