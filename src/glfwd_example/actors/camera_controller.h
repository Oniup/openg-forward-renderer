#pragma once

#include "glfwd_example/actor.h"

namespace glfwd_example {

class CameraController : public Actor
{
public:
    CameraController(const glfwd::ResourceHandle<glfwd::Framebuffer>& render_target,
                     const glm::vec3& position, const glm::vec3& look_at_target = glm::vec3(0.0f),
                     float move_speed = 5.0f, float sensitivity = 2.5f);

    void OnEvent(const SDL_Event& event) override;
    void OnUpdate(const glfwd::Timestep& timestep) override;
    void SubmitToRenderQueue(glfwd::RenderQueue* render_queue) const override;

    glfwd::DirectionalCamera&       GetInternal() { return m_Camera; }
    const glfwd::DirectionalCamera& GetInternal() const { return m_Camera; }

private:
    glfwd::DirectionalCamera                  m_Camera;
    glfwd::ResourceHandle<glfwd::Framebuffer> m_RenderTarget;
    // Controls
    bool  m_HideMouse = false;
    float m_Sensitivity;
    float m_MoveSpeed;
    float m_Yaw;
    float m_Pitch;
};

} // namespace glfwd_example