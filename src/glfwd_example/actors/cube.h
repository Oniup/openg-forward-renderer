#pragma once

#include "glfwd_example/actor.h"

namespace glfwd_example {

class Cube : public Actor
{
public:
    void OnInitialize() override;
    void SubmitToRenderQueue(glfwd::RenderQueue* render_queue) const override;

private:
    glfwd::ResourceHandle<glfwd::Shader> m_ShaderHandle;
    glfwd::ResourceHandle<glfwd::Model>  m_ModelHandle;
};

} // namespace glfwd_example
