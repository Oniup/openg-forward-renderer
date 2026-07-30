#pragma once

#include "glfwd_example/actor.h"

namespace glfwd_example {

class CameraController : public Actor
{
public:
    void OnInitialize() override;
    void OnUpdate(const glfwd::Timestep& timestep) override;
    void SubmitToRenderQueue(glfwd::RenderQueue* render_queue) const override;

private:
    glfwd::TargetCamera                       m_Camera;
    glfwd::ResourceHandle<glfwd::Framebuffer> m_RenderTarget;
};

} // namespace glfwd_example