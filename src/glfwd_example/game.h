#pragma once

#include "glfwd_core/resource_handle.h"
#include "glfwd_core/transform.h"
#include "glfwd_engine/application.h"
#include "glfwd_example/actor.h"

namespace glfwd_example {

class Game : public glfwd::Application
{
public:
    std::vector<std::unique_ptr<Actor>>&       GetActors() { return m_Actors; }
    const std::vector<std::unique_ptr<Actor>>& GetActors() const { return m_Actors; }

protected:
    void OnInitialize() override;
    void OnEvent(const SDL_Event& event) override;
    void OnUpdate(const glfwd::Timestep& timestep) override;
    void SubmitToRenderQueue(glfwd::RenderQueue* render_queue) override;

private:
    std::vector<std::unique_ptr<Actor>>  m_Actors;
    glfwd::TargetCamera                  m_Camera;
    glfwd::Transform                     m_Transform;
    glfwd::Mesh                          m_Mesh = {};
    glfwd::ResourceHandle<glfwd::Shader> m_ShaderHandle;
};

} // namespace glfwd_example
