#pragma once
#include <glm/glm.hpp>
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
    void Zoom(const Event& event);
    void Rotate(const Event& event);
    void MoveForward(const Event& event);
    void MoveBackward(const Event& event);
    void MoveLeft(const Event& event);
    void MoveRight(const Event& event);

    inline static float zoom_speed = 2.0f;
    inline static float rotate_speed = 0.001f;
    inline static float move_speed = 1.0f;
    private:
    float zoom;
    glm::vec3 camera_pos, camera_direction, camera_up, camera_right;
};
}