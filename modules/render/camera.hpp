#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>

namespace lunar{

struct Event;

class Camera{
    public:
    Camera();
    explicit Camera(glm::vec3 camera_pos, float zoom = 45.0f);
    static Camera& getInstance(){ static Camera instance; return instance; }
    [[nodiscard]] glm::mat4 computeViewMatrix() const;
    [[nodiscard]] glm::mat4 computeProjectionMatrix() const;
    [[nodiscard]] glm::vec3 computeTransformMatrix() const;
    void Zoom(Event event);
    void Rotate(Event event);
    void Move(Event event);
    inline static float zoom_speed = 1.0f;
    inline static float rotate_speed = 1.0f;
    inline static float move_speed = 1.0f;
    private:
    float zoom;
    glm::vec3 camera_pos, camera_front, camera_direction, camera_up, camera_right;
};
}