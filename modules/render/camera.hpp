#pragma once
#include "window.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace lunar{
class Camera{
    public:
    Camera();
    Camera(glm::vec3 camera_pos);
    ~Camera();
    static Camera& getInstance(){ static Camera instance; return instance; }
    private:
    glm::vec3 camera_pos, camera_front, camera_direction, camera_up, camera_right;
    glm::mat4 view, projection;
};
}