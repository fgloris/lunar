#include "camera.hpp"

namespace lunar{
    Camera::Camera(glm::vec3 camera_pos) : 
    camera_pos(camera_pos), camera_front(glm::vec3(0.0f, 0.0f, -1.0f)){
        camera_direction = glm::normalize(camera_pos - camera_front);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        camera_right = glm::normalize(glm::cross(up, camera_direction));
        camera_up = glm::normalize(glm::cross(camera_direction, camera_right));
        view = glm::lookAt(camera_pos, camera_pos + camera_front, camera_up);
    }
    Camera::~Camera() {}
}