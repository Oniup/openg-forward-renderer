#pragma once

#include "glfwd_core/resource_handle.h"
#include "glfwd_core/transform.h"
#include "glfwd_renderer/render_queue.h"
#include "glfwd_renderer/resources/model.h"
#include "glfwd_renderer/rhi/shader.h"

namespace glfwd {
class Timestep;
}

union SDL_Event;

namespace glfwd_example {

class Game;

struct Actor
{
    virtual ~Actor() = default;

    glfwd::Transform&       GetTransform() { return m_Transform; }
    const glfwd::Transform& GetTransform() const { return m_Transform; }

    virtual void OnInitialize() = 0;
    virtual void OnEvent(const SDL_Event& event) {}
    virtual void OnUpdate(const glfwd::Timestep& timestep) {}

    virtual void SubmitToRenderQueue(glfwd::RenderQueue* render_queue) const {}

    glfwd::ResourceHandle<glfwd::Shader> ShaderHandle;
    glfwd::ResourceHandle<glfwd::Model>  ModelHandle;

private:
    friend Game;

    Game*            m_Owner = nullptr;
    glfwd::Transform m_Transform;
};

} // namespace glfwd_example